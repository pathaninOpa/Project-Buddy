import asyncio
import websockets
import json
import requests
import logging
from typing import Dict, Set, Optional
import time

# Configure logging
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

class STTBridge:
    def __init__(self, 
                 stt_websocket_url="ws://localhost:8765",
                 http_api_url="http://localhost:5000/transcribe",
                 host="localhost", 
                 port=8766):
        self.stt_websocket_url = stt_websocket_url
        self.http_api_url = http_api_url
        self.host = host
        self.port = port
        self.clients: Set[websockets.WebSocketServerProtocol] = set()
        
        # Statistics
        self.transcription_count = 0
        self.llm_response_count = 0
        self.error_count = 0
        
    async def connect_to_stt_server(self):
        """Connect to the STT WebSocket server"""
        try:
            self.stt_websocket = await websockets.connect(self.stt_websocket_url)
            logger.info(f"Connected to STT server at {self.stt_websocket_url}")
            return True
        except Exception as e:
            logger.error(f"Failed to connect to STT server: {e}")
            return False
    
    async def listen_to_stt_server(self):
        """Listen for transcriptions from STT server"""
        try:
            async for message in self.stt_websocket:
                try:
                    data = json.loads(message)
                    await self.handle_stt_message(data)
                except json.JSONDecodeError:
                    logger.warning(f"Invalid JSON from STT server: {message}")
                except Exception as e:
                    logger.error(f"Error handling STT message: {e}")
        except websockets.exceptions.ConnectionClosed:
            logger.error("Connection to STT server closed")
        except Exception as e:
            logger.error(f"Error listening to STT server: {e}")
    
    async def handle_stt_message(self, data):
        """Handle messages from STT server"""
        message_type = data.get("type")
        
        if message_type == "transcription":
            text = data.get("text", "").strip()
            if text:
                self.transcription_count += 1
                logger.info(f"Received transcription: {text}")
                
                # Forward to HTTP API
                llm_response = await self.forward_to_http_api(text)
                
                if llm_response:
                    self.llm_response_count += 1
                    # Broadcast to all connected clients
                    await self.broadcast_to_clients({
                        "type": "conversation",
                        "transcription": text,
                        "llm_response": llm_response,
                        "timestamp": time.time()
                    })
                else:
                    self.error_count += 1
                    logger.error("Failed to get LLM response")
        
        elif message_type == "status":
            logger.info(f"STT server status: {data.get('message', '')}")
    
    async def forward_to_http_api(self, transcription: str) -> Optional[str]:
        """Forward transcription to HTTP API"""
        try:
            payload = {"text": transcription}
            
            # Use aiohttp for async HTTP requests
            import aiohttp
            async with aiohttp.ClientSession() as session:
                async with session.post(
                    self.http_api_url,
                    json=payload,
                    headers={"Content-Type": "application/json"},
                    timeout=aiohttp.ClientTimeout(total=30)
                ) as response:
                    if response.status == 200:
                        result = await response.json()
                        return result.get('llm_response', '')
                    else:
                        logger.error(f"HTTP API returned status {response.status}")
                        return None
                        
        except Exception as e:
            logger.error(f"Error forwarding to HTTP API: {e}")
            return None
    
    async def handle_client(self, websocket, path):
        """Handle individual client connection"""
        client_id = id(websocket)
        self.clients.add(websocket)
        
        logger.info(f"Client {client_id} connected to bridge. Total clients: {len(self.clients)}")
        
        try:
            # Send welcome message
            await websocket.send(json.dumps({
                "type": "status",
                "message": "Connected to STT-LLM bridge",
                "stt_server": self.stt_websocket_url,
                "http_api": self.http_api_url
            }))
            
            async for message in websocket:
                try:
                    data = json.loads(message)
                    await self.handle_client_message(websocket, data)
                except json.JSONDecodeError:
                    logger.warning(f"Invalid JSON from client {client_id}")
                except Exception as e:
                    logger.error(f"Error processing client message: {e}")
                    
        except websockets.exceptions.ConnectionClosed:
            logger.info(f"Client {client_id} disconnected from bridge")
        except Exception as e:
            logger.error(f"Error handling client {client_id}: {e}")
        finally:
            self.clients.remove(websocket)
            logger.info(f"Client {client_id} cleaned up. Total clients: {len(self.clients)}")
    
    async def handle_client_message(self, websocket, data):
        """Handle messages from clients"""
        message_type = data.get("type")
        
        if message_type == "ping":
            await websocket.send(json.dumps({"type": "pong"}))
        elif message_type == "get_stats":
            await websocket.send(json.dumps({
                "type": "stats",
                "transcription_count": self.transcription_count,
                "llm_response_count": self.llm_response_count,
                "error_count": self.error_count,
                "connected_clients": len(self.clients)
            }))
        else:
            await websocket.send(json.dumps({
                "type": "error",
                "message": f"Unknown message type: {message_type}"
            }))
    
    async def broadcast_to_clients(self, message):
        """Broadcast message to all connected clients"""
        if not self.clients:
            return
        
        message_json = json.dumps(message)
        disconnected_clients = set()
        
        for client in self.clients:
            try:
                await client.send(message_json)
            except websockets.exceptions.ConnectionClosed:
                disconnected_clients.add(client)
            except Exception as e:
                logger.error(f"Error broadcasting to client: {e}")
                disconnected_clients.add(client)
        
        # Remove disconnected clients
        for client in disconnected_clients:
            self.clients.remove(client)
    
    async def start_bridge(self):
        """Start the bridge server"""
        logger.info(f"Starting STT-LLM bridge on {self.host}:{self.port}")
        
        # Connect to STT server
        if not await self.connect_to_stt_server():
            logger.error("Failed to connect to STT server. Exiting.")
            return
        
        # Start listening to STT server in background
        asyncio.create_task(self.listen_to_stt_server())
        
        # Start WebSocket server for clients
        async with websockets.serve(self.handle_client, self.host, self.port):
            logger.info(f"STT-LLM bridge is running on ws://{self.host}:{self.port}")
            await asyncio.Future()  # Run forever
    
    def run(self):
        """Run the bridge in the main thread"""
        asyncio.run(self.start_bridge())

# Convenience function to start the bridge
def start_stt_bridge(stt_websocket_url="ws://localhost:8765", 
                    http_api_url="http://localhost:5000/transcribe",
                    host="localhost", 
                    port=8766):
    """Start the STT-LLM bridge"""
    bridge = STTBridge(stt_websocket_url, http_api_url, host, port)
    bridge.run()

if __name__ == "__main__":
    import argparse
    
    parser = argparse.ArgumentParser(description="STT-LLM Bridge")
    parser.add_argument("--host", default="localhost", help="Host to bind to")
    parser.add_argument("--port", type=int, default=8766, help="Port to bind to")
    parser.add_argument("--stt-url", default="ws://localhost:8765", help="STT WebSocket server URL")
    parser.add_argument("--http-url", default="http://localhost:5000/transcribe", help="HTTP API URL")
    
    args = parser.parse_args()
    
    print(f"Starting STT-LLM bridge on {args.host}:{args.port}")
    print(f"STT WebSocket URL: {args.stt_url}")
    print(f"HTTP API URL: {args.http_url}")
    print("Press Ctrl+C to stop")
    
    try:
        start_stt_bridge(args.stt_url, args.http_url, args.host, args.port)
    except KeyboardInterrupt:
        print("\nBridge stopped") 