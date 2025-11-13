import asyncio
import websockets
import json
import pyaudio
import numpy as np
import wave
import sys
import os

class STTWebSocketClient:
    def __init__(self, server_url="ws://localhost:8765"):
        self.server_url = server_url
        self.websocket = None
        self.sample_rate = 16000
        self.chunk_size = 1024
        self.format = pyaudio.paInt16
        self.channels = 1
        self.p = pyaudio.PyAudio()
        self.stream = None
        
    async def connect(self):
        try:
            self.websocket = await websockets.connect(self.server_url)
            print(f"Connected to STT server at {self.server_url}")
            
            asyncio.create_task(self.listen_for_messages())
            
            return True
        except Exception as e:
            print(f"Failed to connect: {e}")
            return False
    
    async def listen_for_messages(self):
        try:
            async for message in self.websocket:
                try:
                    data = json.loads(message)
                    await self.handle_server_message(data)
                except json.JSONDecodeError:
                    print(f"Received non-JSON message: {message}")
        except websockets.exceptions.ConnectionClosed:
            print("Connection to server closed")
        except Exception as e:
            print(f"Error listening for messages: {e}")
    
    async def handle_server_message(self, data):
        message_type = data.get("type")
        
        if message_type == "status":
            print(f"Server status: {data.get('message', '')}")
        elif message_type == "transcription":
            text = data.get("text", "")
            confidence = data.get("confidence", 0.0)
            if text:
                print(f"🎤 Transcription: {text} (confidence: {confidence:.2f})")
            else:
                print("🎤 No speech detected")
        elif message_type == "audio_received":
            print(f"✅ Audio chunk received: {data.get('status', '')}")
        elif message_type == "error":
            print(f"❌ Server error: {data.get('message', '')}")
        else:
            print(f"📨 Unknown message type: {message_type}")
    
    def start_audio_stream(self):
        try:
            self.stream = self.p.open(
                format=self.format,
                channels=self.channels,
                rate=self.sample_rate,
                input=True,
                frames_per_buffer=self.chunk_size
            )
            print("🎤 Audio stream started")
            return True
        except Exception as e:
            print(f"Failed to start audio stream: {e}")
            return False
    
    async def send_audio_chunks(self):
        """Send audio chunks to the server"""
        if not self.stream:
            print("Audio stream not started")
            return
        
        print("🎤 Recording and sending audio chunks... (Press Ctrl+C to stop)")
        
        try:
            while True:
                audio_chunk = self.stream.read(self.chunk_size, exception_on_overflow=False)
                
                if self.websocket:
                    await self.websocket.send(audio_chunk)
                
                await asyncio.sleep(0.01)
                
        except KeyboardInterrupt:
            print("\n🛑 Stopping audio stream...")
        except Exception as e:
            print(f"Error sending audio: {e}")
        finally:
            if self.stream:
                self.stream.stop_stream()
                self.stream.close()
    
    async def send_test_audio_file(self, audio_file_path):
        if not os.path.exists(audio_file_path):
            print(f"Audio file not found: {audio_file_path}")
            return
        
        print(f"📁 Sending audio file: {audio_file_path}")
        
        try:
            with wave.open(audio_file_path, 'rb') as wf:
                chunk_size = 1024
                while True:
                    audio_chunk = wf.readframes(chunk_size)
                    if not audio_chunk:
                        break
                    
                    if self.websocket:
                        await self.websocket.send(audio_chunk)
                    
                    await asyncio.sleep(0.1)
            
            print("✅ Audio file sent")
            
        except Exception as e:
            print(f"Error sending audio file: {e}")
    
    async def send_control_message(self, message_type, **kwargs):
        if self.websocket:
            message = {"type": message_type, **kwargs}
            await self.websocket.send(json.dumps(message))
    
    async def disconnect(self):
        if self.websocket:
            await self.websocket.close()
        
        if self.stream:
            self.stream.stop_stream()
            self.stream.close()
        
        self.p.terminate()
        print("Disconnected from server")

async def main():
    import argparse
    
    parser = argparse.ArgumentParser(description="STT WebSocket Client")
    parser.add_argument("--server", default="ws://localhost:8765", help="WebSocket server URL")
    parser.add_argument("--mode", choices=["live", "file"], default="live", help="Audio input mode")
    parser.add_argument("--audio-file", help="Audio file to send (for file mode)")
    
    args = parser.parse_args()
    
    client = STTWebSocketClient(args.server)
    
    try:
        if not await client.connect():
            return
        
        if args.mode == "live":
            if client.start_audio_stream():
                await client.send_audio_chunks()
        elif args.mode == "file":
            if args.audio_file:
                await client.send_test_audio_file(args.audio_file)
            else:
                print("Please specify --audio-file for file mode")
        
    except KeyboardInterrupt:
        print("\n🛑 Stopping client...")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        await client.disconnect()

if __name__ == "__main__":
    asyncio.run(main()) 