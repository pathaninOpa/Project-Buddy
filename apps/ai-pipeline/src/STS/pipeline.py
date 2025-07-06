from faster_whisper import WhisperModel
import numpy as np
import webrtcvad
import soundfile as sf
from colorama import Fore, Style, init
import re
from scipy.signal import butter, lfilter
import requests
from google.cloud import texttospeech
import os
from io import BytesIO
from dotenv import load_dotenv
from pathlib import Path
import time
import traceback

load_dotenv()
ttsKey = os.getenv("TTSKEY")
if not ttsKey:
    raise ValueError("google TTS key is not set in the .env file")
full_path = str(Path(ttsKey).resolve())
os.environ["GOOGLE_APPLICATION_CREDENTIALS"] = full_path

class STT:
    def __init__(self):
        self.model = WhisperModel("large-v3", device='cuda', compute_type="int8_float16")
        self.vad = webrtcvad.Vad(1)

    def highpass_filter(self, audio_np, sr=16000, cutoff=100):
        b, a = butter(1, cutoff / (0.5 * sr), btype='high')
        filtered = lfilter(b, a, audio_np.astype(np.float32))
        return filtered.astype(np.float32)

    def normalize_audio(self, audio_np, target_rms=0.1):
        rms = np.sqrt(np.mean(audio_np**2))
        if rms == 0:
            return audio_np
        gain = target_rms / rms
        normalized = audio_np * gain
        return np.clip(normalized, -1.0, 1.0)

    def transcribe_audio(self, buffer):

        audio_np = np.frombuffer(buffer, dtype=np.int16).astype(np.float32) / 32768.0
        audio_np = self.highpass_filter(audio_np)
        audio_np = self.normalize_audio(audio_np)

        buffer_io = BytesIO()
        sf.write(buffer_io, audio_np, samplerate=24000, format='MP3')
        buffer_io.seek(0)

        segments, _ = self.model.transcribe(buffer_io, beam_size=2)
        full_text = ' '.join(segment.text for segment in segments).strip()

        filtered_lines = []
        for sentence in re.split(r'[.!?]', full_text):
            sentence = sentence.strip()
            trigger_words = ["บัดดี้", "buddy", "บอดดี้", "บอดี้", "บัดดี", "บัดดี้้"]

            if any(word in full_text.lower() for word in trigger_words):
                print(f"[STT TRIGGERED TEXT] {full_text}")
                return full_text
            else:
                print(f"[STT SKIPPED TEXT] {full_text}")
                return ""
        return ' '.join(filtered_lines) if filtered_lines else ""

class LLM:
    def __init__(self):
        self.OLLAMA_API_URL = "http://localhost:11434/api/chat"
        self.MODEL_NAME = "gemma3:4b"
        self.SYSTEM_PROMPT = (
            "คุณชื่อบัดดี้ เป็นผู้ช่วย AI สำหรับผู้สูงอายุ พูดภาษาไทยเป็นหลัก ถ้าจำเป็นสามารถใช้คำทับศัพท์ภาษาอังกฤษได้ "
            "ห้ามลงท้ายประโยคด้วยคำว่า 'ค่ะ' หรือ 'คะ' โดยเด็ดขาด "
            "ให้ตอบแบบสบายๆ เป็นธรรมชาติ เหมือนคุยกับคนในครอบครัว "
            "หลีกเลี่ยงการใช้รูปแบบรายการหรือหัวข้อ ให้พูดต่อเนื่องเหมือนบทสนทนาปกติ "
            "เน้นให้คำแนะนำที่ชัดเจน กระชับ และเข้าใจง่าย"
            "พูดให้จบใน 4 ประโยค"
        )

        self.conversation_history = [{"role": "system", "content": self.SYSTEM_PROMPT}]
    
    def inference(self, transcribedText: str) -> str:
        while True:
            text = transcribedText

            self.conversation_history.append({"role": "user", "content": text})
            
            payload = {
                "model": self.MODEL_NAME,
                "messages": self.conversation_history,
                "stream": True,
                "options": {
                    "num_predict": 100  
                }
            }
            
            try:
                with requests.post(self.OLLAMA_API_URL, json=payload, stream=True) as response:
                    response.raise_for_status()
                    assistant_response = ""
                    for line in response.iter_lines():
                        if line:
                            try:
                                data = line.decode("utf-8")
                                import json
                                chunk = json.loads(data)
                                fragment = chunk.get("message", {}).get("content", "")
                                assistant_response += fragment
                            except Exception:
                                continue
                    self.conversation_history.append({"role": "assistant", "content": assistant_response})
                    return assistant_response
            except Exception as e:
                print(f"\n[Error contacting Ollama API]: {e}")

class TTS:
    def __init__(self):
        self.client = texttospeech.TextToSpeechClient()
        self.voice = texttospeech.VoiceSelectionParams(
            language_code="th-TH",
            name="th-TH-Chirp3-HD-Charon",
            ssml_gender=texttospeech.SsmlVoiceGender.FEMALE
        )
    
    def genVoice(self, lmResponse: str) -> bytearray:
        inputText = texttospeech.SynthesisInput(text=lmResponse)
        audio_config = texttospeech.AudioConfig(audio_encoding=texttospeech.AudioEncoding.LINEAR16)
        response = self.client.synthesize_speech(input=inputText,voice=self.voice, audio_config= audio_config)
        print("AI Voice Generated: ",lmResponse)
        return bytearray(response.audio_content)

class RUN:
    def __init__(self):
        self.stt = STT()
        self.llm = LLM()
        self.tts = TTS()
    
    def pipeline(self, audio: bytes) -> bytearray:
        try:
            try:
                transcribed = self.stt.transcribe_audio(audio)
                if not transcribed:
                    print("[STT INFO] No trigger word found — skipping response.")
                    return b""
            except Exception as e:
                print(f"[STT ERROR] {e}")
                return self._fallback_response("ขออภัย ระบบฟังเสียงไม่พร้อมใช้งาน")

            try:
                lmResponse = self.llm.inference(transcribed)
            except Exception as e:
                print(f"[LLM ERROR] {e}")
                return self._fallback_response("ขออภัย ฉันไม่สามารถตอบกลับได้ในขณะนี้")

            try:
                voiceResponse = self.tts.genVoice(lmResponse)
                return voiceResponse
            except Exception as e:
                print(f"[TTS ERROR] {e}")
                return self._fallback_response("ขออภัย ระบบตอบกลับด้วยเสียงไม่พร้อมใช้งาน")

        except Exception as e:
            print(f"[PIPELINE ERROR] {e}")
            traceback.print_exc()
            return self._fallback_response("ขออภัย ระบบเกิดข้อผิดพลาด กรุณาลองใหม่อีกครั้ง")

    def _fallback_response(self, message: str) -> bytearray:
        try:
            return self.tts.genVoice(message)
        except Exception as e:
            print(f"[FALLBACK TTS ERROR] {e}")
            return bytearray()