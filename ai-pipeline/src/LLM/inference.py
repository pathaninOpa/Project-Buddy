import requests

OLLAMA_API_URL = "http://localhost:11434/api/chat"
MODEL_NAME = "gemma3:4b"
SYSTEM_PROMPT = "คุณคือผู้ช่วย AI สำหรับผู้สูงอายุ พูดภาษาไทยได้เท่านั้น ให้คำตอบชัดเจนและกระชับ"

conversation_history = [
    {"role": "system", "content": SYSTEM_PROMPT}
]

print("Ready! Type 'quit' to exit.\n")

while True:
    user_input_text = input("You: ")
    if user_input_text.strip().lower() == "quit":
        print("AI: Goodbye!")        
        break

    conversation_history.append({"role": "user", "content": user_input_text})
    
    payload = {
        "model": MODEL_NAME,
        "messages": conversation_history,
        "stream": True
    }
    
    try:
        with requests.post(OLLAMA_API_URL, json=payload, stream=True) as response:
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
            conversation_history.append({"role": "assistant", "content": assistant_response})
    except Exception as e:
        print(f"\n[Error contacting Ollama API]: {e}")