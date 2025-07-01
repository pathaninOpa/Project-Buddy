from faster_whisper import WhisperModel
import pyaudio
import numpy as np
import webrtcvad
import io
import soundfile as sf
import re
from colorama import Fore, Style, init
import wave
from scipy.signal import butter, lfilter  
import requests
from google.cloud import texttospeech

class STT:
    def __init__(self):
        self.model = WhisperModel("large-v3", device = 'cuda', compute_type = "int8_float16")
        self.vad = webrtcvad.Vad(1) #vad sensitivity
    
    def is_loud_enough(audio_np, threshold=200) -> bool:
        rms = np.sqrt(np.mean(audio_np.astype(np.float32)**2))
        return rms > threshold
    
    def has_speech(self,audio_bytes, sample_rate=16000):
        frame_duration_ms = 10
        bytes_per_sample = 2
        frame_size = int(sample_rate * (frame_duration_ms / 1000.0)) * bytes_per_sample
        for i in range(0, len(audio_bytes) - frame_size, frame_size):
            frame = audio_bytes[i:i + frame_size]
            if self.vad.is_speech(frame, sample_rate):
                return True
        return False
    
    def is_mostly_thai(text, threshold=0.5):
        thai_chars = re.findall(r'[\u0E00-\u0E7F]', text)
        return len(thai_chars) / max(len(text), 1) > threshold
    
    def normalize_audio(audio_np, target_rms=3000):
        rms = np.sqrt(np.mean(audio_np.astype(np.float32)**2))
        if rms == 0:
            return audio_np
        gain = target_rms / rms
        normalized = audio_np.astype(np.float32) * gain
        return np.clip(normalized, -32768, 32767).astype(np.int16)
    
    def highpass_filter(audio_np, sr=16000, cutoff=150):
        b, a = butter(1, cutoff / (0.5 * sr), btype='high')
        filtered = lfilter(b, a, audio_np.astype(np.float32))
        return filtered.astype(np.int16)
    
    def record_stream_chunk(stream, chunk_seconds=1.0):
        frames = []
        for _ in range(int(16000 / 1024 * chunk_seconds)):
            data = stream.read(1024, exception_on_overflow=False)
            frames.append(data)
        return b''.join(frames)
    
    def transcribe_audio_bytes(self, audio_bytes):
        audio_np = np.frombuffer(audio_bytes, dtype=np.int16)

        audio_np = self.highpass_filter(audio_np)

        if np.sqrt(np.mean(audio_np.astype(np.float32)**2)) < 50:
            return ""

        audio_np = self.normalize_audio(audio_np)
        float_audio = audio_np.astype(np.float32) / 32768.0

        buffer = io.BytesIO()
        sf.write(buffer, float_audio, 16000, format='WAV')
        buffer.seek(0)

        segments, _ = self.model.transcribe(buffer, language="th", beam_size=1)
        transcript = ' '.join(segment.text for segment in segments).strip()

        if not transcript or not self.is_mostly_thai(transcript):
            return ""
        return transcript

class LLM:
    def __init__(self):
        self.OLLAMA_API_URL = "http://localhost:11434/api/chat"
        self.MODEL_NAME = "gemma3:4b"
        self.SYSTEM_PROMPT = "คุณคือผู้ช่วย AI สำหรับผู้สูงอายุ พูดภาษาไทยได้เท่านั้น ให้คำตอบชัดเจนและกระชับ"
        self.conversation_history = [{"role": "system", "content": self.SYSTEM_PROMPT}]
    
    def inference(self, transcribedText: str) -> str:
        while True:
            text = transcribedText

            self.conversation_history.append({"role": "user", "content": text})
            
            payload = {
                "model": self.MODEL_NAME,
                "messages": self.conversation_history,
                "stream": True
            }
            
            try:
                with requests.post(self.OLLAMA_API_URL, json=payload, stream=True) as response:
                    response.raise_for_status()
                    print("AI:", end=" ", flush=True)
                    assistant_response = ""
                    for line in response.iter_lines():
                        if line:
                            try:
                                data = line.decode("utf-8")
                                import json
                                chunk = json.loads(data)
                                fragment = chunk.get("message", {}).get("content", "")
                                assistant_response += fragment
                                print(fragment, end="", flush=True)
                            except Exception:
                                continue
                    print()
                    self.conversation_history.append({"role": "assistant", "content": assistant_response})
            except Exception as e:
                print(f"\n[Error contacting Ollama API]: {e}")

class TTS:
    def __init__(self):
        self.client = texttospeech.TextToSpeechClient()
        self.voice = texttospeech.VoiceSelectionParams(
            language_code="th-TH",
            name="th-TH-Chirp3-HD-Charon",
            ssml_gender=texttospeech.SsmlVoiceGender.MALE
        )
    
    def genVoice(self, lmResponse: str) -> bytearray:
        inputText = texttospeech.SynthesisInput(lmResponse)
        audio_config = texttospeech.AudioConfig(audio_encoding=texttospeech.AudioEncoding.LINEAR16)
        response = self.client.synthesize_speech(input=inputText,voice=self.voice, audio_config= audio_config)
        return bytearray(response.audio_content)

class RUN:
    def __init__(self):
        self.stt = STT()
        self.llm = LLM()
        self.tts = TTS()
    
    def pipeline(self, audio:bytes) -> bytearray:
        transcribed = self.stt.transcribe_audio_bytes(audio)
        lmResponse = self.llm.inference(transcribed)
        voiceResponse = self.tts.genVoice(lmResponse)
        return voiceResponse