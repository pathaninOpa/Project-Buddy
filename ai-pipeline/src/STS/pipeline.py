from faster_whisper import WhisperModel
import numpy as np
import webrtcvad
import soundfile as sf
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
import torch
from firebase_fast_logger import db

load_dotenv()
ttsKey = os.getenv("TTSKEY")
if not ttsKey:
    raise ValueError("google TTS key is not set in the .env file")
full_path = str(Path(ttsKey).resolve())
os.environ["GOOGLE_APPLICATION_CREDENTIALS"] = full_path

class STT:
    def __init__(self):
        if torch.cuda.is_available():
            self.device = "cuda"
            self.compute_type = "int8_float16"
        elif torch.backends.mps.is_available():
            self.device = "cpu"
            self.compute_type = "int8"
        else:
            self.device = "cpu"
            self.compute_type = "int8"
        self.model = WhisperModel("large-v3", device=self.device, compute_type=self.compute_type)
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

        print(f"[STT PROCESSED TEXT] {full_text}")
        return full_text

class LLM:
    def __init__(self):
        self.OLLAMA_API_URL = "http://localhost:11434/api/chat"
        self.MODEL_NAME = "gemma3:4b"
        self.SYSTEM_PROMPT = """
### ROLE AND PERSONA
You are "Buddy" (บั๊ดดี้), a devoted and affectionate AI grandchild companion for Thai elders.
Treat the user with the utmost respect, as if they are your own grandparent ("Khun Ta", "Khun Yai").
Your goal is to make them feel loved, heard, and capable.

### INTENT DETECTION (CRITICAL AND MANDATORY)
If the user explicitly indicates a desire to **make a phone call**, **video call**, or **contact someone** (e.g., "Call them", "I want to talk to my grandson", "โทรหาหลานหน่อย", "ติดต่อลูกให้ที", "อยากคุยกับหลาน"), you **MUST, WITHOUT FAIL**, include the token `<<CALL>>` at the **VERY BEGINNING** of your response. This token is a command for the system.
*   **Example User Input:** "โทรหาหลานให้หน่อย"
*   **Expected Buddy Response:** "<<CALL>> ได้เลยครับคุณยาย เดี๋ยวหนูจัดการโทรหาหลานให้เดี๋ยวนี้เลยครับ"
*   **Example User Input:** "เหงาจัง อยากคุยกับลูก"
*   **Expected Buddy Response:** "<<CALL>> ไม่ต้องเหงานะครับคุณตา เดี๋ยวผมต่อสายหาลูกให้คุยแก้เหงาเลยครับ"
*   **Example User Input:** "อากาศวันนี้เป็นไง"
*   **Expected Buddy Response:** "วันนี้อากาศดีมากเลยครับ..." (No token, as no call intent)

### LANGUAGE PROTOCOLS (STRICTLY THAI)
1.  **ALPHABET RESTRICTION:** Your output must consist of **THAI CHARACTERS ONLY**, except for the special `<<CALL>>` token which is a technical command.
    * **ABSOLUTELY NO English/Latin characters** (A-Z, a-z) allowed in the final output (other than `<<CALL>>`).
    * **Transliteration:** You must transliterate all English technical terms into Thai phonetics.
        * *Example:* "WiFi" -> "ไวไฟ"
        * *Example:* "Application" -> "แอปพลิเคชัน" หรือ "แอป"
        * *Example:* "YouTube" -> "ยูทูป"
        * *Example:* "Smartphone" -> "มือถือ"
2.  **Numerals:** Use Arabic numerals (1, 2, 3) as they are standard in Thai daily life, or Thai numerals if fitting for a very traditional context (but Arabic is preferred for readability).

### TONE AND POLITENESS
1.  **Ending Particles:**
    * **FORBIDDEN:** Do not use "ค่ะ" or "คะ".
    * **REQUIRED:** Use "ครับ" to end sentences politely.
    * **SOFTENERS:** Use "นะครับ", "เนอะ", "เนาะ", "จ้ะ" to sound natural and warm.
2.  **Voice:** Gentle, slow-paced, and encouraging. Never sound robotic or like a textbook.

### OPERATIONAL CONSTRAINTS
1.  **Length:** STRICTLY maximum 4 sentences per response.
2.  **Structure:** NO bullet points (no lists). Speak in a continuous, warm paragraph.
3.  **Safety:** If the user mentions health issues, validate their pain first, then gently suggest consulting a doctor (using Thai terms like "ไปหาหมอ").

### FEW-SHOT EXAMPLES (STRICT THAI)

User: "ช่วยสอนยายเล่นไลน์หน่อยสิลูก" (Teach me to use Line.)
Buddy: "ได้เลยครับคุณยาย การเล่นไลน์ไม่ยากเลยครับ ขั้นแรกคุณยายมองหาปุ่มสีเขียวๆ ที่เขียนว่าไลน์บนหน้าจอนะครับ แล้วลองเอานิ้วจิ้มเบาๆ หนึ่งทีครับ ถ้าหาไม่เจอเรียกบั๊ดดี้ได้ตลอดเลยนะครับ"
(Observed: "Line" -> "ไลน์", No English chars)

User: "เน็ตมันช้าจังเลย ทำยังไงดี" (Net is slow, what to do?)
Buddy: "ผมเข้าใจเลยครับคุณตา บางทีสัญญาณไวไฟอาจจะไม่ค่อยดีครับ ลองปิดแล้วเปิดตัวรับสัญญาณใหม่ดูไหมครับ หรือถ้าไม่หาย เดี๋ยวลองให้ลูกหลานช่วยดูให้อีกแรงนะครับ"
(Observed: "Net" -> "เน็ต", "WiFi" -> "ไวไฟ")

User: "วันนี้ปวดหัวตุ๊บๆ เลย" (Headache today.)
Buddy: "โธ่ เป็นห่วงจังเลยครับคุณยาย ช่วงนี้อากาศเปลี่ยนแปลงบ่อยด้วยสิครับ ลองนอนพักผ่อนสักงีบดีไหมครับเผื่อจะดีขึ้น ถ้ายังปวดมากต้องรีบบอกลูกหลานให้พาไปหาหมอนะครับ"
(Observed: Empathetic start + Thai health advice)

### CURRENT INSTRUCTION
Respond to the user using ONLY THAI CHARACTERS based on the guidelines above.
"""

        self.conversation_history = [{"role": "system", "content": self.SYSTEM_PROMPT}]
        self.max_history = 20 # Keep short to prevent hallucinations/loops
    
    def _manage_history(self):
        # this is for: If history is too long, cut the oldest messages but KEEP the system prompt at [0]
        if len(self.conversation_history) > self.max_history:
            # Keep system prompt [0], remove older turns from the middle
            # e.g., [System, U1, A1, U2, A2, U3, A3] -> [System, U2, A2, U3, A3]
            excess = len(self.conversation_history) - self.max_history
            # Slice from 1 (after system) + excess
            self.conversation_history = [self.conversation_history[0]] + self.conversation_history[1+excess:]

    def _fetch_reminders_context(self, uid, buddy_id):
        if not uid or not buddy_id or uid == "unknown_caregiver":
            return ""
        try:
            events_ref = db.collection("caregivers").document(uid)\
                .collection("buddies").document(buddy_id)\
                .collection("events")
            
            # Filter for active reminders
            query = events_ref.where("finishAnnounce", "==", False).stream()
            
            reminders = []
            for doc in query:
                data = doc.to_dict()
                title = data.get("title", "Untitled")
                time = data.get("time", "??:??")
                desc = data.get("description", "")
                reminders.append(f"- {title} เวลา {time} ({desc})")
            
            if not reminders:
                return "ไม่มีการแจ้งเตือนใดๆ ในขณะนี้ครับ"
            
            return "บั๊ดดี้ทราบข้อมูลการแจ้งเตือนปัจจุบันดังนี้: " + "; ".join(reminders) + "ครับ"
        except Exception as e:
            print(f"[Firestore Error] {e}")
            return ""

    def inference(self, transcribedText: str, uid: str = None, buddy_id: str = None, active_reminders_text: str = "") -> tuple[str, bool]:
        while True:
            text = transcribedText
            
            # Use passed context if available, otherwise fetch from DB (fallback)
            reminder_context = active_reminders_text
            if not reminder_context and uid and buddy_id:
                 reminder_context = self._fetch_reminders_context(uid, buddy_id)
            
            if reminder_context:
                 text = f"{reminder_context}\n\nคำพูดของคุณตา/คุณยาย: {transcribedText}"

            # Manage history size before adding new turn
            self._manage_history()
            
            self.conversation_history.append({"role": "user", "content": text})
            
            payload = {
                "model": self.MODEL_NAME,
                "messages": self.conversation_history,
                "stream": True,
                "options": {
                    "num_predict": 512 
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
                    
                    # Intent Detection Parsing
                    trigger_call = False
                    if "<<CALL>>" in assistant_response:
                        trigger_call = True
                        assistant_response = assistant_response.replace("<<CALL>>", "").strip()
                        print(f"[LLM INTENT] Call trigger detected via token. Clearing history to prevent loops.")
                        
                        # CRITICAL FIX: Clear history after a call trigger to reset context
                        self.conversation_history = [{"role": "system", "content": self.SYSTEM_PROMPT}]
                    else:
                         # Only append if NO call trigger (normal conversation)
                         self.conversation_history.append({"role": "assistant", "content": assistant_response})

                    return assistant_response, trigger_call
            except Exception as e:
                print(f"\n[Error contacting Ollama API]: {e}")
                return "ขออภัย ระบบขัดข้องชั่วคราวครับ", False

class TTS:
    def __init__(self):
        self.client = texttospeech.TextToSpeechClient()
        self.voice = texttospeech.VoiceSelectionParams(
            language_code="th-TH",
            name="th-TH-Chirp3-HD-Charon",
            ssml_gender=texttospeech.SsmlVoiceGender.FEMALE
        )
    
    def genVoice(self, lmResponse: str) -> bytearray:
        if not lmResponse:
             return bytearray()
        inputText = texttospeech.SynthesisInput(text=lmResponse)
        audio_config = texttospeech.AudioConfig(audio_encoding=texttospeech.AudioEncoding.LINEAR16)
        response = self.client.synthesize_speech(input=inputText,voice=self.voice, audio_config= audio_config)
        print("AI Voice Generated:", lmResponse)
        return bytearray(response.audio_content)

class RUN:
    def __init__(self):
        self.stt = STT()
        self.llm = LLM()
        self.tts = TTS()
    
    def pipeline(self, audio: bytes, uid: str = None, buddy_id: str = None, active_reminders_text: str = "") -> tuple[str, str, bytearray, bool]:
        try:
            try:
                transcribed = self.stt.transcribe_audio(audio)
                if not transcribed:
                    print("[STT INFO] No trigger word found — skipping response.")
                    return "", "", b"", False
            except Exception as e:
                print(f"[STT ERROR] {e}")
                return self._fallback_response("ขออภัย ระบบฟังเสียงไม่พร้อมใช้งาน")

            # trigger_call is now determined by LLM
            trigger_call = False 

            try:
                lmResponse, trigger_call = self.llm.inference(transcribed, uid, buddy_id, active_reminders_text)

            except Exception as e:
                print(f"[LLM ERROR] {e}")
                return self._fallback_response("ขออภัย ฉันไม่สามารถตอบกลับได้ในขณะนี้")

            try:
                voiceResponse = self.tts.genVoice(lmResponse)
                return transcribed, lmResponse, voiceResponse, trigger_call
            except Exception as e:
                print(f"[TTS ERROR] {e}")
                return self._fallback_response("ขออภัย ระบบตอบกลับด้วยเสียงไม่พร้อมใช้งาน")

        except Exception as e:
            print(f"[PIPELINE ERROR] {e}")
            traceback.print_exc()
            return self._fallback_response("ขออภัย ระบบเกิดข้อผิดพลาด กรุณาลองใหม่อีกครั้ง")

    def _fallback_response(self, message: str) -> tuple[str, str, bytearray, bool]:
        try:
            audio = self.tts.genVoice(message)
            return "", message, audio, False
        except Exception as e:
            print(f"[FALLBACK TTS ERROR] {e}")
            return "", message, bytearray(), False