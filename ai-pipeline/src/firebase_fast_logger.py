import firebase_admin
from firebase_admin import credentials, firestore
from datetime import datetime
import logging

# Initialize Firebase once
# Make sure 'firebase-key.json' is in the same folder or provide full path
if not firebase_admin._apps:
    cred = credentials.Certificate("firebase-key.json") 
    firebase_admin.initialize_app(cred)

db = firestore.client()

class ChatLogger:
    def log_chat(self, user_id, user_text, ai_text):
        """
        Logs the conversation pair (User + AI) to Firebase
        Status: is_analyzed = False (waiting for the background analyst)
        """
        try:
            today_str = datetime.now().strftime("%Y-%m-%d")
            
            # Path: users/{user_id}/daily_sessions/{date}/messages/
            messages_ref = db.collection("users").document(user_id)\
                .collection("daily_sessions").document(today_str)\
                .collection("messages")

            # Log User Message
            messages_ref.add({
                "role": "user",
                "content": user_text,
                "timestamp": firestore.SERVER_TIMESTAMP,
                "is_analyzed": False,
                "emotion": None
            })

            # Log AI Response
            messages_ref.add({
                "role": "assistant",
                "content": ai_text,
                "timestamp": firestore.SERVER_TIMESTAMP,
                "is_analyzed": True,
                "emotion": "neutral"
            })
            
            logging.info(f"Logged chat for user {user_id}")
            
        except Exception as e:
            logging.error(f"Firebase Error: {e}")