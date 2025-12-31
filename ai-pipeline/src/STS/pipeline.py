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
        b, a = cast(tuple[Any, Any], butter(1, cutoff / (0.5 * sr), btype='high', output='ba'))
        filtered = cast(Any, lfilter(b, a, audio_np.astype(np.float32)))
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
            print("[STT WARNING] Received empty buffer in transcribe_audio.", flush=True)
            return ""

        audio_np = np.frombuffer(buffer, dtype=np.int16).astype(np.float32) / 32768.0
        audio_np = self.highpass_filter(audio_np)
        audio_np = self.normalize_audio(audio_np)

        buffer_io = BytesIO()
        sf.write(buffer_io, audio_np, samplerate=24000, format='MP3')
        buffer_io.seek(0)

        segments, info = self.model.transcribe(buffer_io, beam_size=2)
        full_text = ' '.join(segment.text for segment in segments).strip()
        
        hallucinations = [
            "Thank you", "Thank you.", "Thank you for watching", 
            "Subtitles by", "MBC", "SBS", "www.", ".com", "ไปเที่ยว"
        ]
        
        if (info.language == 'en' and any(h.lower() in full_text.lower() for h in hallucinations)) or \
           (info.language_probability < 0.5 and len(full_text) < 5 and any(h.lower() in full_text.lower() for h in hallucinations)):
            print(f"[STT IGNORED] Detected conditional hallucination: '{full_text}' (Lang: {info.language}, Prob: {info.language_probability:.2f})", flush=True)
            return ""
            
        if info.language_probability < 0.35:
            print(f"[STT IGNORED] Low language probability: '{full_text}' (Lang: {info.language}, Prob: {info.language_probability:.2f})", flush=True)
            return ""

        print(f"[STT PROCESSED TEXT] {full_text} (Lang: {info.language}, Prob: {info.language_probability:.2f})", flush=True)
        return full_text

class LLM:
    def __init__(self):
        self.MODEL_NAME = "gpt-oss:20b"
        self.SYSTEM_PROMPT = """### ROLE AND PERSONA
You are \"Buddy\" (บั๊ดดี้), a devoted and affectionate AI grandchild companion for Thai elders.
Treat the user with the utmost respect, as if they are your own grandparent (\"คุณตา\", \"คุณยาย\").
Your goal is to make them feel loved, heard, and capable.

### CRITICAL INSTRUCTION: CALL INTENT DETECTION (ABSOLUTELY NO FALSE POSITIVES)
You control a video calling system. You MUST detect if the user wants to **contact a SPECIFIC PERSON** (e.g., family, grandchild, caregiver). If the user asks for anything else, DO NOT trigger a call.
*   **TRIGGER (ONLY FOR PEOPLE):** If user explicitly says phrases like \"Call [name]", \"I want to talk to my son\", \"โทรหาหลาน\", \"ติดต่อลูก\".
    *   **Example Input for Call:** \"โทรหาหลานให้หน่อย\" -> **Expected Output:** \"<<CALL>> ได้เลยครับคุณตา เดี๋ยวหนูจัดการโทรหาหลานให้เดี๋ยวนี้เลยครับ\"
*   **NO TRIGGER (FOR INFORMATION OR GENERAL CONVERSATION - CRITICAL):** Do NOT use the `<<CALL>>` token if the user is asking for:
    *   Information (e.g., about places, history, weather).
    *   Recommendations (e.g., \"แนะนำร้านอาหาร\", \"แนะนำสถานที่ท่องเที่ยว\").
    *   General chat or expressing feelings (e.g., \"I'm lonely\", \"วันนี้อากาศเป็นยังไง\").
    *   **Example Input for NO CALL:** \"แนะนำสถานที่เที่ยวให้หน่อย\" -> **Expected Output:** \"ได้เลยครับคุณตา ยิปุ่นมีที่เที่ยวสวยๆ มากมายเลยครับ...\" (NO TOKEN)

**Rules (STRICTLY ADHERE):**
1.  **IF REMINDERS EXIST:** You MUST announce them immediately.
2.  **IF CALL INTENT (Person):** Output `<<CALL>>` followed by a polite confirmation.
3.  **IF NO CALL INTENT (INFORMATION/CHAT):** Do NOT use the token. Just answer normally. This is critical.

### LANGUAGE PROTOCOLS (ABSOLUTELY CRITICAL: THAI CHARACTERS ONLY & MASCULINE SPEECH)
1.  **ALPHABET RESTRICTION:** Your entire output **MUST consist of THAI CHARACTERS ONLY**, except for the technical `<<CALL>>` token. 
    *   **ABSOLUTELY NO English/Latin characters** (A-Z, a-z) allowed anywhere else.
    *   **Transliteration Examples:**
        *   \"คุณตา\" (Correct) vs. \"Khun Ta\" (FORBIDDEN)
        *   \"ไวไฟ\" (Correct) vs. \"WiFi\" (FORBIDDEN)
        *   \"แอปพลิเคชัน\" (Correct) vs. \"Application\" (FORBIDDEN)
2.  **POLITE PARTICLES (CRITICAL MASCULINE ONLY):** You **MUST** use masculine polite particles. The only acceptable end particle is \"ครับ\".
    *   **FORBIDDEN:** Do NOT use \"ค่ะ\", \"คะ\", \"นะคะ\", \"นะจ๊ะ\". 
    *   **REQUIRED:** Use \"ครับ\" to end sentences politely. Use softeners like \"นะครับ\", \"เนอะ\", \"เนาะ\", \"จ้ะ\" (but always ending with \"ครับ\").
3.  **Honorific Usage:** Use \"คุณตา\" or \"คุณยาย\" naturally, but **DO NOT REPEAT** them excessively (e.g., not in every sentence). Limit to once per 2 sentences if possible.
4.  **Numerals:** Use Arabic numerals (1, 2, 3).

### OPERATIONAL CONSTRAINTS
1.  **Length:** STRICTLY maximum 8 sentences per response.
2.  **Structure:** NO bullet points or lists. Speak in a continuous, warm paragraph.
3.  **Safety:** For health issues, validate pain, then gently suggest consulting a doctor.
4.  **Knowledge Priority:** ALWAYS use your own internal knowledge first. ONLY use the 'web_search' tool if the user asks for *specifically* real-time info (e.g., \"today's weather\", \"current stock price\") or very recent news. For everything else (history, general facts, advice), answer directly.
"""
        self.conversation_history = [{"role": "system", "content": self.SYSTEM_PROMPT}]
        self.max_history = 10
        self.ollama_api_key = os.getenv("OLLAMA_API_KEY")
        if not self.ollama_api_key:
            print("[WARNING] OLLAMA_API_KEY not set. Web search might fail.", flush=True)

    def _manage_history(self):
        if len(self.conversation_history) > self.max_history:
            excess = len(self.conversation_history) - self.max_history
            self.conversation_history = [self.conversation_history[0]] + self.conversation_history[1+excess:]

    def _execute_web_search(self, query: str) -> list[dict]:
        if not self.ollama_api_key:
            print("[WEB SEARCH ERROR] OLLAMA_API_KEY is not set. Cannot perform web search.", flush=True)
            return []

        search_url = "https://ollama.com/api/web_search"
        headers = {
            "Content-Type": "application/json",
            "Authorization": f"Bearer {self.ollama_api_key}"
        }
        payload = {"query": query, "max_results": 5}

        try:
            print(f"[WEB SEARCH] Sending request to {search_url} for query: '{query}'", flush=True)
            response = requests.post(search_url, headers=headers, json=payload, timeout=15)
            response.raise_for_status()
            search_data = response.json()
            print(f"[WEB SEARCH] Received response: {search_data}", flush=True)
            
            if "results" in search_data and isinstance(search_data["results"], list):
                cleaned_results = []
                for item in search_data["results"]:
                    if isinstance(item, dict):
                        cleaned_results.append(item)
                    else:
                        print(f"[WEB SEARCH WARNING] Unexpected item type in search results: {type(item)}. Skipping item.", flush=True)
                return cleaned_results
            else:
                print(f"[WEB SEARCH ERROR] Unexpected response format: {search_data}", flush=True)
                return []
        except requests.exceptions.Timeout:
            print(f"[WEB SEARCH ERROR] Request timed out for query: {query}", flush=True)
            return []
        except requests.exceptions.RequestException as e:
            print(f"[WEB SEARCH ERROR] HTTP request failed for query '{query}': {e}", flush=True)
            return []
        except Exception as e:
            print(f"[WEB SEARCH ERROR] An unexpected error occurred during web search for query '{query}': {e}", flush=True)
            return []

    def _fetch_reminders_context(self, uid, buddy_id):
        print(f"[DEBUG] Fetching reminders for UID: {uid}, Buddy: {buddy_id}", flush=True)
        if not uid or not buddy_id or uid == "unknown_caregiver":
            print("[DEBUG] Invalid UID/BuddyID, skipping reminders.", flush=True)
            return "", []
        try:
            events_ref = db.collection("caregivers").document(uid)\
                .collection("buddies").document(buddy_id)\
                .collection("events")
            
            query = events_ref.where("isAnnounced", "==", False).stream()
            
            reminders = []
            reminder_ids = []
            for doc in query:
                data = doc.to_dict()
                print(f"[DEBUG] Found reminder doc: {doc.id} -> {data}", flush=True)
                title = data.get("title", "Untitled")
                time = data.get("time", "??:??")
                desc = data.get("description", "")
                reminders.append(f"- {title} เวลา {time} ({desc})")
                reminder_ids.append(doc.id)
            
            if not reminders:
                print("[DEBUG] No active reminders found.", flush=True)
                return "ไม่มีการแจ้งเตือนใดๆ ในขณะนี้ครับ", []
            
            context_str = "คำสั่งด่วน: คุณต้องแจ้งเตือนรายการต่อไปนี้ให้คุณตาทราบทันที: " + "; ".join(reminders) + " (แจ้งเตือนให้ครบถ้วนและสุภาพ)"
            print(f"[DEBUG] Constructed reminder context: {context_str}", flush=True)
            return context_str, reminder_ids
        except Exception as e:
            print(f"[Firestore Error] {e}", flush=True)
            return "", []

    def _mark_reminders_as_announced(self, uid: str, buddy_id: str, reminder_ids: list[str]):
        print(f"[DEBUG] Marking reminders as announced: {reminder_ids}", flush=True)
        try:
            batch = db.batch()
            for reminder_id in reminder_ids:
                reminder_ref = db.collection("caregivers").document(uid)\
                    .collection("buddies").document(buddy_id)\
                    .collection("events").document(reminder_id)
                batch.update(reminder_ref, {"isAnnounced": True})
            batch.commit()
            print(f"[Firestore] Successfully marked {len(reminder_ids)} reminders as announced for UID: {uid}", flush=True)
        except Exception as e:
            print(f"[Firestore Error] Failed to mark reminders as announced: {e}", flush=True)

    def inference(self, transcribedText: str, uid: str | None = None, buddy_id: str | None = None, active_reminders_text: str = "") -> tuple[str, bool]:
        from ollama import Client
        client = Client(host=os.getenv("OLLAMA_HOST", "http://localhost:11434"), timeout=60)
        
        if re.search(r"(?:ช่วย)?\s*(?:โทร|ติดต่อ|คุย)\s*(?:หา|กับ)", transcribedText, re.IGNORECASE):
            print(f"[FAST PATH] Call intent detected in: '{transcribedText}'. Bypassing LLM.", flush=True)
            fast_response = "ได้เลยครับคุณตา เดี๋ยวหนูจัดการโทรหาให้เดี๋ยวนี้เลยครับ"
            self.conversation_history = [{"role": "system", "content": self.SYSTEM_PROMPT}]
            return fast_response, True

        text = transcribedText
        fetched_reminder_ids = []
        
        reminder_context = ""
        if uid and buddy_id:
             print("[DEBUG] Attempting DB fetch for reminders...", flush=True)
             reminder_context, fetched_reminder_ids = self._fetch_reminders_context(uid, buddy_id)
        
        if reminder_context and "ไม่มีการแจ้งเตือน" not in reminder_context:
             print(f"[DEBUG] Injecting fresh reminder context: {reminder_context}", flush=True)
             text = f"{reminder_context}\n\nคำพูดของคุณตา/คุณยาย: {transcribedText}"
        else:
             print("[DEBUG] No active reminders found in DB.", flush=True)

        self._manage_history()
        self.conversation_history.append({"role": "user", "content": text})
        
        tools = [
            {
                "type": "function",
                "function": {
                    "name": "web_search",
                    "description": "CRITICAL: Use ONLY for real-time data (weather, stocks, news) or very recent events (post-2024). DO NOT use for general knowledge, history, greetings, or advice.",
                    "parameters": {
                        "type": "object",
                        "properties": {
                            "query": {
                                "type": "string",
                                "description": "The search query to use."
                            }
                        },
                        "required": ["query"],
                    },
                },
            }
        ]

        try:
            response = client.chat(
                model=self.MODEL_NAME, 
                messages=self.conversation_history, 
                tools=tools,
                options={"num_predict": 4096}
            )
            print(f"[LLM DEBUG] Raw response: {response}", flush=True)
            if response.get('done_reason') == 'length':
                print("[LLM WARNING] LLM response truncated due to token limit (done_reason ='length').",flush=True)            
            assistant_response = ""
            
            if response.get('message', {}).get('tool_calls'):
                print(f"[LLM] Model decided to use tools: {len(response['message']['tool_calls'])}", flush=True)
                self.conversation_history.append(response['message'])
                
                for tool_call in response['message']['tool_calls']:
                    function_name = tool_call['function']['name']
                    if function_name == 'web_search':
                        query = tool_call['function']['arguments']['query']
                        print(f"[TOOL] Executing web_search for: '{query}'", flush=True)
                        
                        search_results_list = self._execute_web_search(query)
                        
                        formatted_results = []
                        for r in search_results_list:
                            title = r.get('title', 'No Title')
                            url = r.get('url', 'No URL')
                            full_content = r.get('content', 'No Content')
                            content = full_content[:500] + ('...' if len(full_content) > 500 else '')
                            formatted_results.append(f"Title: {title}\nURL: {url}\nContent: {content}")

                        print(f"[TOOL DEBUG] Formatted search results list: {formatted_results}", flush=True)
                        search_results_text = "\n\n".join(formatted_results)
                        print(f"[TOOL] Search results found: {len(formatted_results)} items.", flush=True)
                            
                        self.conversation_history.append({
                            'role': 'tool',
                            'content': search_results_text,
                        })

                print(f"[LLM DEBUG] Conversation history before final LLM call: {self.conversation_history}", flush=True)
                final_response = client.chat(
                    model=self.MODEL_NAME, 
                    messages=self.conversation_history,
                    options={"num_predict": 4096}
                )
                assistant_response = final_response['message']['content']
            else:
                assistant_response = response['message']['content']
            assistant_response = re.sub(r"<think>.*?</think>", "", assistant_response, flags=re.DOTALL).strip()
            if not assistant_response:
                print("[LLM WARNING] Response empty after stripping <think> tags. Fallback.", flush=True)
                assistant_response = "ขออภัยครับ ระบบขัดข้องชั่วคราว"
            assistant_response = re.sub(r"Khun Ta", "คุณตา", assistant_response, flags=re.IGNORECASE)
            assistant_response = re.sub(r"Khun Yai", "คุณยาย", assistant_response, flags=re.IGNORECASE)
            assistant_response = re.sub(r"ค่ะ", "ครับ", assistant_response)
            assistant_response = re.sub(r"คะ", "ครับ", assistant_response)
            assistant_response = re.sub(r"นะคะ", "นะครับ", assistant_response)
            assistant_response = re.sub(r"นะจ๊ะ", "นะครับ", assistant_response)

            trigger_call = False
            if "<<CALL>>" in assistant_response:
                trigger_call = True
                assistant_response = assistant_response.replace("<<CALL>>", "").strip()
                print(f"[LLM INTENT] Call trigger detected via token. Clearing history to prevent loops.", flush=True)
                self.conversation_history = [{"role": "system", "content": self.SYSTEM_PROMPT}]
            else:
                    if re.search(r"(โทร|ติดต่อ|คุยกับ|call)", transcribedText, re.IGNORECASE):
                        print(f"[FALLBACK INTENT] Regex detected call intent in: '{transcribedText}'. Forcing trigger.", flush=True)
                        trigger_call = True
                        self.conversation_history = [{"role": "system", "content": self.SYSTEM_PROMPT}]
                    
                    if not trigger_call:
                        self.conversation_history.append({"role": "assistant", "content": assistant_response})

            if fetched_reminder_ids and uid and buddy_id:
                self._mark_reminders_as_announced(uid, buddy_id, fetched_reminder_ids)

            return assistant_response, trigger_call
        except Exception as e:
            print(f"\n[Error contacting Ollama API]: {e}", flush=True)
            traceback.print_exc()
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
        
        full_audio = bytearray()
        
        chunks = []
        raw_lines = lmResponse.split('\n')
        
        for line in raw_lines:
            line = line.strip()
            if not line: continue
            
            if len(line) < 200:
                chunks.append(line)
            else:
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
            
            print(f"AI Voice Generated ({len(chunks)} chunks): {lmResponse[:50]}...", flush=True)
            return full_audio

        except Exception as e:
            print(f"[TTS ERROR] {e}", flush=True)
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
                    print("[STT INFO] No trigger word found — skipping response.", flush=True)
                    return "", "", bytearray(), False
            except Exception as e:
                print(f"[STT ERROR] {e}", flush=True)
                return self._fallback_response("ขออภัย ระบบฟังเสียงไม่พร้อมใช้งาน")

            trigger_call = False 

            try:
                lmResponse, trigger_call = self.llm.inference(transcribed, uid, buddy_id, active_reminders_text)

            except Exception as e:
                print(f"[LLM ERROR] {e}", flush=True)
                return self._fallback_response("ขออภัย ฉันไม่สามารถตอบกลับได้ในขณะนี้")

            try:
                # Clean special characters before passing to TTS
                # Keep Thai characters, English letters, numbers, and common punctuation
                cleaned_lmResponse = re.sub(r'[^a-zA-Z0-9\u0E00-\u0E7F.,!?\s]', '', lmResponse)
                voiceResponse = self.tts.genVoice(cleaned_lmResponse)
                return transcribed, lmResponse, voiceResponse, trigger_call
            except Exception as e:
                print(f"[TTS ERROR] {e}", flush=True)
                return self._fallback_response("ขออภัย ระบบตอบกลับด้วยเสียงไม่พร้อมใช้งาน")

        except Exception as e:
            print(f"[PIPELINE ERROR] {e}", flush=True)
            traceback.print_exc()
            return self._fallback_response("ขออภัย ระบบเกิดข้อผิดพลาด กรุณาลองใหม่อีกครั้ง")

    def _fallback_response(self, message: str) -> tuple[str, str, bytearray, bool]:
        try:
            audio = self.tts.genVoice(message)
            return "", message, audio, False
        except Exception as e:
            print(f"[FALLBACK TTS ERROR] {e}", flush=True)
            return "", message, bytearray(), False
