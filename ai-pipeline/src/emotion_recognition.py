import time
import schedule
import firebase_admin
from firebase_admin import credentials, firestore
import requests
import logging
import os

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

if not firebase_admin._apps:
    default_key_path = "../secrets/firebase-key-buddy.json" 
    key_path = os.getenv("FIREBASE_KEY_PATH", default_key_path)

    if os.path.exists(key_path):
        cred = credentials.Certificate(key_path)
        firebase_admin.initialize_app(cred)
    else:
        logging.error(f"Firebase Key not found at: {key_path}")
        exit(1)

db = firestore.client()

OLLAMA_URL = "http://localhost:11434/api/chat"
MODEL_NAME = "gemma3:1b" 

def get_emotion_from_llm(text):
    """
    Sends text to Local LLM to detect emotion.
    """
    prompt = (
        f"Analyze the emotion of this text spoken by an elderly person: '{text}'. "
        "Return ONLY ONE word from this list: [Neutral, Happy, Relax, Sad, Pain, Anxiety, Angry]. "
        "Do not add punctuation."
    )
    try:
        response = requests.post(OLLAMA_URL, json={
            "model": MODEL_NAME,
            "messages": [{"role": "user", "content": prompt}],
            "stream": False,
            "options": {"temperature": 0.0}
        })
        if response.status_code == 200:
            return response.json()["message"]["content"].strip()
        return "Error"
    except Exception as e:
        logging.error(f"LLM Connection Error: {e}")
        return "Unknown"

def analyze_job():
    logging.info("Starting Batch Emotion Recognition...")
    docs = db.collection_group("messages")\
             .where("is_analyzed", "==", False)\
             .where("role", "==", "user")\
             .limit(50)\
             .stream()

    count = 0
    for doc in docs:
        data = doc.to_dict()
        text = data.get("content")
        
        if text:
            logging.info(f"   Analyzing: {text[:30]}...")
            emotion = get_emotion_from_llm(text)
            doc.reference.update({
                "emotion": emotion,
                "is_analyzed": True,
                "analyzed_at": firestore.SERVER_TIMESTAMP
            })
            logging.info(f"   -> Result: {emotion}")
            count += 1
            
    if count == 0:
        logging.info("No new messages to analyze.")
    else:
        logging.info(f"Job finished. Processed {count} messages.")

schedule.every(15).minutes.do(analyze_job)

if __name__ == "__main__":
    logging.info("Emotion Recognition Service Started")
    
    # Run once immediately on startup for testing
    analyze_job()
    
    while True:
        schedule.run_pending()
        time.sleep(1)