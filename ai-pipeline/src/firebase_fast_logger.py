import firebase_admin
from firebase_admin import credentials, firestore
from datetime import datetime
import logging
import os

# Initialize Firebase once
if not firebase_admin._apps:
    default_key_path = "secrets/firebase-key-buddy.json" 
    key_path = os.getenv("FIREBASE_KEY_PATH", default_key_path)

    if os.path.exists(key_path):
        cred = credentials.Certificate(key_path)
        firebase_admin.initialize_app(cred)
    else:
        logging.error(f"Firebase Key not found at: {key_path}")

db = firestore.client()

class ChatLogger:
    def log_chat(self, uid, buddyID, user_text, ai_text):
        """
        Logs the conversation pair (User + AI) to Firebase
        Path: caregivers/{uid}/buddies/{buddyID}/daily_sessions/{date}/messages/{auto_id}
        """
        try:
            today_str = datetime.now().strftime("%Y-%m-%d")
            
            # Point to the session document (The Date Document)
            session_doc_ref = db.collection("caregivers").document(uid)\
                .collection("buddies").document(buddyID)\
                .collection("daily_sessions").document(today_str)
            
            session_doc_ref.set({
                "last_updated": firestore.SERVER_TIMESTAMP,
            }, merge=True) 

            # Point to the 'messages' sub-collection inside that session
            messages_collection = session_doc_ref.collection("messages")

            # Log User Message
            messages_collection.add({
                "content": user_text,
                "emotion": None,
                "is_analyzed": False,
                "role": "user",
                "timestamp": firestore.SERVER_TIMESTAMP
            })

            # Log AI Response
            messages_collection.add({
                "content": ai_text,
                "emotion": "neutral",
                "is_analyzed": True,
                "role": "assistant",
                "timestamp": firestore.SERVER_TIMESTAMP
            })
            
            logging.info(f"✅ Logged chat for Caregiver: {uid} | Buddy: {buddyID}")
            
        except Exception as e:
            logging.error(f"🔥 Firebase Error: {e}")