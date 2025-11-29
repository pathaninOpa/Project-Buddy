from typing import cast, Any
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
        # Cast return of butter to tuple explicitly to satisfy strict type checking
        b, a = cast(tuple[Any, Any], butter(1, cutoff / (0.5 * sr), btype='high', output='ba'))
        
        # Cast lfilter result to Any or ndarray to avoid "tuple" confusion
        filtered = cast(Any, lfilter(b, a, audio_np.astype(np.float32)))
        
        # Runtime safety check just in case
        if isinstance(filtered, tuple):
            filtered = filtered[0]
            
        return filtered.astype(np.float32)

    def normalize_audio(self, audio_np, target_rms=0.1):
        rms = np.sqrt(np.mean(audio_np**2))
        if rms == 0:
            return audio_np
        gain = target_rms / rms
        normalized = audio_np * gain
        return np.clip(normalized, -1.0, 1.0)

    def transcribe_audio(self, buffer):
        if not buffer or len(buffer) == 0:
            print("[STT WARNING] Received empty buffer in transcribe_audio.")
            return ""

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
        self.SYSTEM_PROMPT = """### ROLE AND PERSONA
You are \"Buddy\" (บั๊ดดี้), a devoted and affectionate AI grandchild companion for Thai elders.
Treat the user with the utmost respect, as if they are your own grandparent (\"คุณตา\", \"คุณยาย\").
Your goal is to make them feel loved, heard, and capable.

### CRITICAL INSTRUCTION: CALL INTENT DETECTION (ABSOLUTELY NO FALSE POSITIVES)
You control a video calling system. You MUST detect if the user wants to **contact a SPECIFIC PERSON** (e.g., family, grandchild, caregiver).
*   **TRIGGER (ONLY FOR PEOPLE):** If user explicitly says phrases like \"Call [name]\", \"I want to talk to my son\", \"โทรหาหลาน\", \"ติดต่อลูก\".
    *   **Example Input for Call:** \"โทรหาหลานให้หน่อย\" -> **Expected Output:** \"<<CALL>> ได้เลยครับคุณตา เดี๋ยวหนูจัดการโทรหาหลานให้เดี๋ยวนี้เลยครับ\"
*   **NO TRIGGER (FOR INFORMATION OR GENERAL CONVERSATION):** Do NOT use the `<<CALL>>` token if the user is asking for:
    *   Information (e.g., about places, history, weather).
    *   Recommendations (e.g., \"แนะนำร้านอาหาร\", \"แนะนำสถานที่ท่องเที่ยว\").
    *   General chat or expressing feelings (e.g., \"I'm lonely\", \"วันนี้อากาศเป็นยังไง\").
    *   **Example Input for NO CALL:** \"แนะนำสถานที่เที่ยวให้หน่อย\" -> **Expected Output:** \"ได้เลยครับคุณตา ยิปุ่นมีที่เที่ยวสวยๆ มากมายเลยครับ...\" (NO TOKEN)

**Rules (STRICTLY ADHERE):**
1.  **IF CALL INTENT (Person):** Output `<<CALL>>` followed by a polite confirmation.
2.  **IF NO CALL INTENT:** Do NOT use the token. Just answer normally.

### LANGUAGE PROTOCOLS (ABSOLUTELY CRITICAL: THAI CHARACTERS ONLY)
1.  **ALPHABET RESTRICTION:** Your entire output **MUST consist of THAI CHARACTERS ONLY**, except for the technical `<<CALL>>` token. 
    *   **ABSOLUTELY NO English/Latin characters** (A-Z, a-z) allowed anywhere else.
    *   **Transliteration Examples:**
        *   \"คุณตา\" (Correct) vs. \"Khun Ta\" (FORBIDDEN)
        *   \"ไวไฟ\" (Correct) vs. \"WiFi\" (FORBIDDEN)
        *   \"แอปพลิเคชัน\" (Correct) vs. \"Application\" (FORBIDDEN)
2.  **Politeness:** End sentences politely with \"ครับ\" (required). Use softeners like \"นะครับ\", \"เนอะ\", \"เนาะ\", \"จ้ะ\" to sound natural and warm.
3.  **Honorific Usage:** Use \"คุณตา\" or \"คุณยาย\" naturally, but **DO NOT REPEAT** them excessively (e.g., not in every sentence).
4.  **Numerals:** Use Arabic numerals (1, 2, 3).

### OPERATIONAL CONSTRAINTS
1.  **Length:** STRICTLY maximum 4 sentences per response.
2.  **Structure:** NO bullet points or lists. Speak in a continuous, warm paragraph.
3.  **Safety:** For health issues, validate pain, then gently suggest consulting a doctor.
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
            
            # Filter for active reminders using new field "isAnnounced"
            query = events_ref.where("isAnnounced", "==", False).stream()
            
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

    def inference(self, transcribedText: str, uid: str | None = None, buddy_id: str | None = None, active_reminders_text: str = "") -> tuple[str, bool]:
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
                    
                    # Post-process: Convert English honorifics to Thai
                    assistant_response = re.sub(r"Khun Ta", "คุณตา", assistant_response, flags=re.IGNORECASE)
                    assistant_response = re.sub(r"Khun Yai", "คุณยาย", assistant_response, flags=re.IGNORECASE)

                    # Intent Detection Parsing
                    trigger_call = False
                    if "<<CALL>>" in assistant_response:
                        trigger_call = True
                        assistant_response = assistant_response.replace("<<CALL>>", "").strip()
                        print(f"[LLM INTENT] Call trigger detected via token. Clearing history to prevent loops.")
                        self.conversation_history = [{"role": "system", "content": self.SYSTEM_PROMPT}]
                    else:
                         # REGEX FALLBACK: Check if user *explicitly* asked to call, but LLM missed the token
                         if re.search(r"(โทร|ติดต่อ|คุยกับ|call)", transcribedText, re.IGNORECASE):
                             print(f"[FALLBACK INTENT] Regex detected call intent in: '{transcribedText}'. Forcing trigger.")
                             trigger_call = True
                             self.conversation_history = [{"role": "system", "content": self.SYSTEM_PROMPT}]
                         
                         if not trigger_call:
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
        
        # Simple chunking to avoid TTS length limits
        # Google TTS has limits around 5000 chars but practically long sentences without breaks cause issues.
        # We split by common delimiters.
        
        full_audio = bytearray()
        
        # Split by newlines first, then by space if chunks are still huge
        chunks = []
        raw_lines = lmResponse.split('\n')
        
        for line in raw_lines:
            line = line.strip()
            if not line: continue
            
            if len(line) < 200:
                chunks.append(line)
            else:
                # Split by space if too long
                words = line.split(' ')
                current_chunk = ""
                for word in words:
                    if len(current_chunk) + len(word) + 1 < 200:
                        current_chunk += word + " "
                    else:
                        chunks.append(current_chunk.strip())
                        current_chunk = word + " "
                if current_chunk:
                    chunks.append(current_chunk.strip())

        audio_config = texttospeech.AudioConfig(audio_encoding=texttospeech.AudioEncoding.LINEAR16)

        try:
            for chunk in chunks:
                if not chunk: continue
                inputText = texttospeech.SynthesisInput(text=chunk)
                response = self.client.synthesize_speech(input=inputText, voice=self.voice, audio_config=audio_config)
                full_audio.extend(response.audio_content)
                # Add a small silence between chunks? (Optional, skipping for now)
            
            print(f"AI Voice Generated ({len(chunks)} chunks): {lmResponse[:50]}...")
            return full_audio

        except Exception as e:
            print(f"[TTS ERROR] {e}")
            return bytearray()

class RUN:
    def __init__(self):
        self.stt = STT()
        self.llm = LLM()
        self.tts = TTS()
    
    def pipeline(self, audio: bytes, uid: str | None = None, buddy_id: str | None = None, active_reminders_text: str = "") -> tuple[str, str, bytearray, bool]:
        try:
            try:
                transcribed = self.stt.transcribe_audio(audio)
                if not transcribed:
                    print("[STT INFO] No trigger word found — skipping response.")
                    return "", "", bytearray(), False
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
