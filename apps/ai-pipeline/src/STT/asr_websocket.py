import asyncio
import websockets
import json
import numpy as np
import io
import soundfile as sf
import re
from faster_whisper import WhisperModel
import webrtcvad
from scipy.signal import butter, lfilter
from typing import Dict, Set
import logging

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class STTWebSocketServer:
    def __init__(self, host="localhost", port=8765):
        self.host = host
        self.port = port
        self.clients: Set[websockets.WebSocketServerProtocol] = set()
        self.vad = webrtcvad.Vad(1)
        self.model = WhisperModel("large-v3", device="cuda", compute_type="int8_float16")
        self.sample_rate = 16000
        self.chunk_duration_ms = 1000 
        self.silence_threshold_ms = 3000  
        self.client_buffers: Dict[websockets.WebSocketServerProtocol, bytearray] = {}
        self.client_silence_timers: Dict[websockets.WebSocketServerProtocol, float] = {}
        
    def is_loud_enough(self, audio_np, threshold=200):
        rms = np.sqrt(np.mean(audio_np.astype(np.float32)**2))
        return rms > threshold
    
    def has_speech(self, audio_bytes):
        frame_duration_ms = 10
        bytes_per_sample = 2
        frame_size = int(self.sample_rate * (frame_duration_ms / 1000.0)) * bytes_per_sample
        
        for i in range(0, len(audio_bytes) - frame_size, frame_size):
            frame = audio_bytes[i:i + frame_size]
            if self.vad.is_speech(frame, self.sample_rate):
                return True
        return False
    
    def is_mostly_thai(self, text, threshold=0.5):
        thai_chars = re.findall(r'[\u0E00-\u0E7F]', text)
        return len(thai_chars) / max(len(text), 1) > threshold
    
    def normalize_audio(self, audio_np, target_rms=3000):
        rms = np.sqrt(np.mean(audio_np.astype(np.float32)**2))
        if rms == 0:
            return audio_np
        gain = target_rms / rms
        normalized = audio_np.astype(np.float32) * gain
        return np.clip(normalized, -32768, 32767).astype(np.int16)
    
    def highpass_filter(self, audio_np, cutoff=150):
        b, a = butter(1, cutoff / (0.5 * self.sample_rate), btype='high')
        filtered = lfilter(b, a, audio_np.astype(np.float32))
        return filtered.astype(np.int16)
    
    def transcribe_audio_bytes(self, audio_bytes):
        try:
            audio_np = np.frombuffer(audio_bytes, dtype=np.int16)
            
            if np.sqrt(np.mean(audio_np.astype(np.float32)**2)) < 50: #skip if too quiet
                return ""
            
            audio_np = self.highpass_filter(audio_np)
            audio_np = self.normalize_audio(audio_np)
            float_audio = audio_np.astype(np.float32) / 32768.0
            
            buffer = io.BytesIO()
            sf.write(buffer, float_audio, self.sample_rate, format='WAV')
            buffer.seek(0)
            
            segments, _ = self.model.transcribe(buffer, language="th", beam_size=1)
            transcript = ' '.join(segment.text for segment in segments).strip()
            
            if not transcript or not self.is_mostly_thai(transcript):
                return ""
            
            return transcript
            
        except Exception as e:
            logger.error(f"Transcription error: {e}")
            return ""
    
    async def handle_client(self, websocket, path):
        """Handle individual client connection"""
        client_id = id(websocket)
        self.clients.add(websocket)
        self.client_buffers[websocket] = bytearray()
        self.client_silence_timers[websocket] = 0.0
        
        logger.info(f"Client {client_id} connected. Total clients: {len(self.clients)}")
        
        try:
            await websocket.send(json.dumps({
                "type": "status",
                "message": "Connected to STT server",
                "sample_rate": self.sample_rate,
                "chunk_duration_ms": self.chunk_duration_ms
            }))
            
            async for message in websocket:
                try:
                    if isinstance(message, bytes):
                        await self.process_audio_chunk(websocket, message)
                    else:
                        data = json.loads(message)
                        await self.handle_json_message(websocket, data)
                        
                except json.JSONDecodeError:
                    logger.warning(f"Invalid JSON from client {client_id}")
                except Exception as e:
                    logger.error(f"Error processing message from client {client_id}: {e}")
                    
        except websockets.exceptions.ConnectionClosed:
            logger.info(f"Client {client_id} disconnected")
        except Exception as e:
            logger.error(f"Error handling client {client_id}: {e}")
        finally:
            self.clients.remove(websocket)
            if websocket in self.client_buffers:
                del self.client_buffers[websocket]
            if websocket in self.client_silence_timers:
                del self.client_silence_timers[websocket]
            logger.info(f"Client {client_id} cleaned up. Total clients: {len(self.clients)}")
    
    async def process_audio_chunk(self, websocket, audio_chunk):
        client_id = id(websocket)
        buffer = self.client_buffers[websocket]
        silence_timer = self.client_silence_timers[websocket]
        
        audio_np = np.frombuffer(audio_chunk, dtype=np.int16)
        
        if self.is_loud_enough(audio_np) and self.has_speech(audio_chunk):
            self.client_silence_timers[websocket] = 0.0
            buffer.extend(audio_chunk)
            
            await websocket.send(json.dumps({
                "type": "audio_received",
                "status": "speech_detected"
            }))
            
        else:
            self.client_silence_timers[websocket] += self.chunk_duration_ms / 1000.0
            
            if silence_timer >= (self.silence_threshold_ms / 1000.0) and len(buffer) > 0:
                transcript = self.transcribe_audio_bytes(buffer)
                
                if transcript:
                    await websocket.send(json.dumps({
                        "type": "transcription",
                        "text": transcript,
                        "confidence": 1.0 #todo: add confidence handle logic
                    }))
                    logger.info(f"Client {client_id}: {transcript}")
                else:
                    await websocket.send(json.dumps({
                        "type": "transcription",
                        "text": "",
                        "confidence": 0.0
                    }))
                
                buffer.clear()
                self.client_silence_timers[websocket] = 0.0
    
    async def handle_json_message(self, websocket, data):
        message_type = data.get("type")
        
        if message_type == "ping":
            await websocket.send(json.dumps({"type": "pong"}))
        elif message_type == "reset":
            self.client_buffers[websocket].clear()
            self.client_silence_timers[websocket] = 0.0
            await websocket.send(json.dumps({"type": "status", "message": "Buffer reset"}))
        elif message_type == "get_status":
            await websocket.send(json.dumps({
                "type": "status",
                "buffer_size": len(self.client_buffers[websocket]),
                "silence_timer": self.client_silence_timers[websocket]
            }))
        else:
            await websocket.send(json.dumps({
                "type": "error",
                "message": f"Unknown message type: {message_type}"
            }))
    
    async def start_server(self):
        logger.info(f"Starting STT WebSocket server on {self.host}:{self.port}")
        
        async with websockets.serve(self.handle_client, self.host, self.port):
            logger.info(f"STT WebSocket server is running on ws://{self.host}:{self.port}")
            await asyncio.Future() 
    
    def run(self):
        asyncio.run(self.start_server())

def start_stt_websocket_server(host="localhost", port=8765):
    server = STTWebSocketServer(host, port)
    server.run()

if __name__ == "__main__":
    import argparse
    
    parser = argparse.ArgumentParser(description="STT WebSocket Server")
    parser.add_argument("--host", default="localhost", help="Host to bind to")
    parser.add_argument("--port", type=int, default=8765, help="Port to bind to")
    
    args = parser.parse_args()
    
    print(f"Starting STT WebSocket server on {args.host}:{args.port}")
    print("Clients can connect and send PCM audio chunks for real-time transcription")
    print("Press Ctrl+C to stop")
    
    try:
        start_stt_websocket_server(args.host, args.port)
    except KeyboardInterrupt:
        print("\nServer stopped") 