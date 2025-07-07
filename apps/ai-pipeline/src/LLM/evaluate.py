import csv
import json
import requests
from datetime import datetime
from dotenv import load_dotenv
from pathlib import Path
import os
from google.generativeai import GenerativeModel, configure

load_dotenv()
gemAPI = os.getenv("GEMINIAPI")
if not gemAPI:
    raise ValueError("GEMINI API key is not set in the .env file")
# ----------------------
# CONFIGURATION
# ----------------------
script_dir = Path(__file__).parent.resolve()
output_dir = script_dir / "OUT"
output_dir.mkdir(parents=True, exist_ok=True)

PROMPT_CSV_FILE = script_dir / "prompts.csv"
JSON_OUTPUT_FILE = output_dir / "llm_eval_batch.json"
MARKDOWN_OUTPUT_FILE = output_dir / "llm_eval_batch.md"
CSV_OUTPUT_FILE = output_dir / "llm_eval_batch.csv"

# System Prompt
SYSTEM_PROMPT = "คุณคือผู้ช่วย AI สำหรับผู้สูงอายุ พูดภาษาไทยได้เท่านั้น ให้คำตอบชัดเจนและกระชับ"

# Ollama (Local)
OLLAMA_API_URL = "http://localhost:11434/api/chat"
OLLAMA_MODEL = "gemma3:4b"

# Gemma 3 27b (Google API)
configure(api_key=gemAPI)
gemma27b_model = GenerativeModel("gemma-3-27b-it")

# Gemini (Google API)
configure(api_key=gemAPI)
gemini_model = GenerativeModel("gemini-2.5-flash")

# ----------------------
# LOAD PROMPTS FROM CSV
# ----------------------

prompts = []
with open(PROMPT_CSV_FILE, newline='', encoding="utf-8") as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        cat = row.get("category", "").strip()
        p = row.get("prompt", "").strip()
        if p:
            prompts.append({"category": cat, "prompt": p})

# ----------------------
# BATCH EVALUATION
# ----------------------

results = []

for idx, entry in enumerate(prompts, 1):
    user_prompt = entry["prompt"]
    category = entry["category"]
    print(f"[{idx}/{len(prompts)}] Evaluating ({category}): {user_prompt}")

    # --- Gemma (Ollama) ---
    gemma_response = ""
    try:
        payload = {
            "model": OLLAMA_MODEL,
            "messages": [
                {"role": "system", "content": SYSTEM_PROMPT},
                {"role": "user", "content": user_prompt}
            ],
            "stream": True
        }
        with requests.post(OLLAMA_API_URL, json=payload, stream=True) as response:
            response.raise_for_status()
            for line in response.iter_lines():
                if line:
                    chunk = json.loads(line.decode("utf-8"))
                    gemma_response += chunk.get("message", {}).get("content", "")
    except Exception as e:
        gemma_response = f"[ERROR]: {e}"

    # --- Gemma 3 27b ---
    try:
        gemma27b_response = gemma27b_model.generate_content([SYSTEM_PROMPT, user_prompt]).text
    except Exception as e:
        gemma27b_response = f"[ERROR]: {e}"

    # --- Gemini 2.5 flash ---
    evaluation_prompt = f"""
คุณเป็นนักประเมินโมเดลภาษา AI โปรดประเมินคำตอบจาก 2 โมเดลภาษา (Model A = gemma3:4b, Model B = Gemini 2.5) จากด้านล่างนี้ โดยใช้เกณฑ์: 
- ความชัดเจน
- ความเกี่ยวข้องกับผู้สูงอายุ
- ความกระชับ

### Prompt:
{user_prompt}

### Model A (gemma3:4b):
{gemma_response}

### Model B (gemma3:27b):
{gemma27b_response}

กรุณาให้คะแนนแต่ละโมเดล (1-10) พร้อมคำอธิบายอย่างกระชับเป็นภาษาไทย
"""

    try:
        evaluation = gemini_model.generate_content([
            "คุณคือผู้เชี่ยวชาญในการประเมินคุณภาพโมเดล AI",
            evaluation_prompt
        ]).text
    except Exception as e:
        evaluation = f"[ERROR during evaluation]: {e}"


    # --- Save result ---
    record = {
        "timestamp": datetime.now().isoformat(),
        "category": category,
        "prompt": user_prompt,
        "gemma3:4b_response": gemma_response.strip(),
        "gemma3:27b_response": gemma27b_response.strip(),
        "evaluation": evaluation.strip()
    }
    results.append(record)

# ----------------------
# SAVE OUTPUTS
# ----------------------

# JSON
with open(JSON_OUTPUT_FILE, "w", encoding="utf-8") as jf:
    json.dump(results, jf, ensure_ascii=False, indent=2)

# Markdown
with open(MARKDOWN_OUTPUT_FILE, "w", encoding="utf-8") as mf:
    last_cat = None
    for r in results:
        # Print category as header when it changes
        if r["category"] != last_cat:
            mf.write(f"# 📂 Category: {r['category']}\n\n")
            last_cat = r["category"]

        mf.write(f"## 🧩 Prompt: {r['prompt']}\n\n")
        mf.write(f"### 🤖 gemma3:4b\n{r['gemma3:4b_response']}\n\n")
        mf.write(f"### 🤖 Gemma3:27b\n{r['gemma3:27b_response']}\n\n")
        mf.write(f"### 🧠 Evaluation by Gemini-2.5-flash\n{r['evaluation']}\n\n")
        mf.write("---\n\n")

# CSV
with open(CSV_OUTPUT_FILE, "w", newline='', encoding="utf-8") as cf:
    writer = csv.writer(cf)
    writer.writerow(["Timestamp", "Category", "Prompt", "Gemma3:4B Response", "Gemma3:27B Response", "Gemini-2.5-flash"])
    for r in results:
        writer.writerow([
            r["timestamp"],
            r["category"],
            r["prompt"],
            r["gemma3:4b_response"],
            r["gemma3:27b_response"],
            r["evaluation"]
        ])

print("\n✅ Batch evaluation complete. Files saved:")
print(f"- {JSON_OUTPUT_FILE}")
print(f"- {MARKDOWN_OUTPUT_FILE}")
print(f"- {CSV_OUTPUT_FILE}")
