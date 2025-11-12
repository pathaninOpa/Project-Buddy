from flask import Flask, request, jsonify
import requests
import json
import logging
from typing import Optional
import asyncio
import threading
import time

# Configure logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class STTHTTPAPI:
    def __init__(self, llm_service_url="http://localhost:8000/generate", host="localhost", port=5000):
        self.llm_service_url = llm_service_url
        self.host = host
        self.port = port
        self.app = Flask(__name__)
        self.setup_routes()
        
        # Statistics
        self.request_count = 0
        self.success_count = 0
        self.error_count = 0
        
    def setup_routes(self):
        """Setup Flask routes"""
        
        @self.app.route('/health', methods=['GET'])
        def health_check():
            """Health check endpoint"""
            return jsonify({
                "status": "healthy",
                "service": "STT-HTTP-API",
                "timestamp": time.time(),
                "stats": {
                    "total_requests": self.request_count,
                    "successful_requests": self.success_count,
                    "failed_requests": self.error_count
                }
            })
        
        @self.app.route('/transcribe', methods=['POST'])
        def transcribe():
            """Receive transcription from STT and forward to LLM"""
            try:
                self.request_count += 1
                
                # Get JSON data
                data = request.get_json()
                if not data:
                    return jsonify({"error": "No JSON data provided"}), 400
                
                # Extract transcription text
                transcription = data.get('text', '').strip()
                if not transcription:
                    return jsonify({"error": "No transcription text provided"}), 400
                
                logger.info(f"Received transcription: {transcription}")
                
                # Forward to LLM service
                llm_response = self.forward_to_llm(transcription)
                
                if llm_response:
                    self.success_count += 1
                    return jsonify({
                        "status": "success",
                        "transcription": transcription,
                        "llm_response": llm_response,
                        "timestamp": time.time()
                    })
                else:
                    self.error_count += 1
                    return jsonify({
                        "status": "error",
                        "message": "Failed to get response from LLM service",
                        "transcription": transcription
                    }), 500
                    
            except Exception as e:
                self.error_count += 1
                logger.error(f"Error processing transcription: {e}")
                return jsonify({"error": str(e)}), 500
        
        @self.app.route('/batch_transcribe', methods=['POST'])
        def batch_transcribe():
            """Handle multiple transcriptions at once"""
            try:
                self.request_count += 1
                
                data = request.get_json()
                if not data:
                    return jsonify({"error": "No JSON data provided"}), 400
                
                transcriptions = data.get('transcriptions', [])
                if not transcriptions:
                    return jsonify({"error": "No transcriptions provided"}), 400
                
                logger.info(f"Received {len(transcriptions)} transcriptions")
                
                # Process each transcription
                results = []
                for transcription in transcriptions:
                    if transcription.strip():
                        llm_response = self.forward_to_llm(transcription)
                        results.append({
                            "transcription": transcription,
                            "llm_response": llm_response,
                            "timestamp": time.time()
                        })
                
                self.success_count += 1
                return jsonify({
                    "status": "success",
                    "results": results,
                    "count": len(results)
                })
                
            except Exception as e:
                self.error_count += 1
                logger.error(f"Error processing batch transcriptions: {e}")
                return jsonify({"error": str(e)}), 500
    
    def forward_to_llm(self, transcription: str) -> Optional[str]:
        """Forward transcription to LLM service"""
        try:
            # Prepare request payload
            payload = {
                "prompt": transcription,
                "max_tokens": 150,
                "temperature": 0.7,
                "stream": False
            }
            
            # Make request to LLM service
            response = requests.post(
                self.llm_service_url,
                json=payload,
                headers={"Content-Type": "application/json"},
                timeout=30
            )
            
            if response.status_code == 200:
                result = response.json()
                return result.get('response', result.get('text', ''))
            else:
                logger.error(f"LLM service returned status {response.status_code}: {response.text}")
                return None
                
        except requests.exceptions.RequestException as e:
            logger.error(f"Request to LLM service failed: {e}")
            return None
        except Exception as e:
            logger.error(f"Error forwarding to LLM: {e}")
            return None
    
    def run(self, debug=False):
        """Run the Flask server"""
        logger.info(f"Starting STT HTTP API server on {self.host}:{self.port}")
        logger.info(f"LLM service URL: {self.llm_service_url}")
        
        self.app.run(
            host=self.host,
            port=self.port,
            debug=debug,
            threaded=True
        )

# Convenience function to start the server
def start_stt_http_api(llm_service_url="http://localhost:8000/generate", host="localhost", port=5000, debug=False):
    """Start the STT HTTP API server"""
    api = STTHTTPAPI(llm_service_url, host, port)
    api.run(debug)

if __name__ == "__main__":
    import argparse
    
    parser = argparse.ArgumentParser(description="STT HTTP API Server")
    parser.add_argument("--host", default="localhost", help="Host to bind to")
    parser.add_argument("--port", type=int, default=5000, help="Port to bind to")
    parser.add_argument("--llm-url", default="http://localhost:8000/generate", help="LLM service URL")
    parser.add_argument("--debug", action="store_true", help="Enable debug mode")
    
    args = parser.parse_args()
    
    print(f"Starting STT HTTP API server on {args.host}:{args.port}")
    print(f"LLM service URL: {args.llm_url}")
    print("Press Ctrl+C to stop")
    
    try:
        start_stt_http_api(args.llm_url, args.host, args.port, args.debug)
    except KeyboardInterrupt:
        print("\nServer stopped") 