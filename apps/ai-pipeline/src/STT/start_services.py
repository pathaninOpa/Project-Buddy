#!/usr/bin/env python3
"""
STT Services Startup Script

This script starts all the STT-related services:
1. STT WebSocket Server
2. HTTP API Server  
3. STT-LLM Bridge (optional)

Usage:
    python start_services.py [options]
"""

import subprocess
import sys
import time
import signal
import os
from typing import List, Dict
import argparse

class ServiceManager:
    def __init__(self):
        self.processes: List[subprocess.Popen] = []
        self.service_configs: Dict[str, Dict] = {
            'stt_server': {
                'script': 'asr_websocket.py',
                'args': ['--host', 'localhost', '--port', '8765'],
                'name': 'STT WebSocket Server',
                'required': True
            },
            'http_api': {
                'script': 'stt_http_api.py',
                'args': ['--host', 'localhost', '--port', '5000', '--llm-url', 'http://localhost:8000/generate'],
                'name': 'HTTP API Server',
                'required': True
            },
            'bridge': {
                'script': 'stt_bridge.py',
                'args': ['--host', 'localhost', '--port', '8766', '--stt-url', 'ws://localhost:8765', '--http-url', 'http://localhost:5000/transcribe'],
                'name': 'STT-LLM Bridge',
                'required': False
            }
        }
    
    def start_service(self, service_key: str) -> bool:
        """Start a specific service"""
        config = self.service_configs[service_key]
        script_path = os.path.join(os.path.dirname(__file__), config['script'])
        
        if not os.path.exists(script_path):
            print(f"❌ Error: {config['script']} not found at {script_path}")
            return False
        
        try:
            print(f"🚀 Starting {config['name']}...")
            process = subprocess.Popen(
                [sys.executable, script_path] + config['args'],
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1,
                universal_newlines=True
            )
            
            self.processes.append(process)
            print(f"✅ {config['name']} started (PID: {process.pid})")
            
            # Give the service a moment to start
            time.sleep(2)
            
            # Check if process is still running
            if process.poll() is None:
                return True
            else:
                stdout, stderr = process.communicate()
                print(f"❌ {config['name']} failed to start:")
                if stdout:
                    print(f"STDOUT: {stdout}")
                if stderr:
                    print(f"STDERR: {stderr}")
                return False
                
        except Exception as e:
            print(f"❌ Error starting {config['name']}: {e}")
            return False
    
    def start_all_services(self, include_bridge: bool = False) -> bool:
        """Start all required services and optionally the bridge"""
        print("🎯 Starting STT Services...")
        print("=" * 50)
        
        # Start required services
        for service_key, config in self.service_configs.items():
            if config['required']:
                if not self.start_service(service_key):
                    print(f"❌ Failed to start required service: {config['name']}")
                    self.stop_all_services()
                    return False
        
        # Start bridge if requested
        if include_bridge:
            if not self.start_service('bridge'):
                print("⚠️  Warning: Failed to start bridge service")
        
        print("=" * 50)
        print("✅ All services started successfully!")
        print("\n📋 Service URLs:")
        print(f"   STT WebSocket: ws://localhost:8765")
        print(f"   HTTP API: http://localhost:5000")
        if include_bridge:
            print(f"   Bridge: ws://localhost:8766")
        print(f"\n🔧 Test with: python test_websocket_client.py --server ws://localhost:8765 --mode live")
        print("Press Ctrl+C to stop all services")
        
        return True
    
    def stop_all_services(self):
        """Stop all running services"""
        print("\n🛑 Stopping all services...")
        
        for process in self.processes:
            try:
                if process.poll() is None:  # Process is still running
                    process.terminate()
                    print(f"   Terminated process {process.pid}")
                    
                    # Wait for graceful shutdown
                    try:
                        process.wait(timeout=5)
                    except subprocess.TimeoutExpired:
                        process.kill()
                        print(f"   Force killed process {process.pid}")
                        
            except Exception as e:
                print(f"   Error stopping process {process.pid}: {e}")
        
        self.processes.clear()
        print("✅ All services stopped")
    
    def check_services(self) -> bool:
        """Check if all services are running"""
        for process in self.processes:
            if process.poll() is not None:
                return False
        return True
    
    def monitor_services(self):
        """Monitor services and restart if needed"""
        try:
            while True:
                if not self.check_services():
                    print("⚠️  One or more services stopped unexpectedly")
                    break
                time.sleep(5)
        except KeyboardInterrupt:
            pass

def signal_handler(signum, frame):
    """Handle interrupt signals"""
    print("\n🛑 Received interrupt signal")
    if hasattr(signal_handler, 'service_manager'):
        signal_handler.service_manager.stop_all_services()
    sys.exit(0)

def main():
    parser = argparse.ArgumentParser(description="STT Services Startup Script")
    parser.add_argument("--include-bridge", action="store_true", help="Include STT-LLM bridge service")
    parser.add_argument("--stt-port", type=int, default=8765, help="STT WebSocket server port")
    parser.add_argument("--http-port", type=int, default=5000, help="HTTP API server port")
    parser.add_argument("--bridge-port", type=int, default=8766, help="Bridge server port")
    parser.add_argument("--llm-url", default="http://localhost:8000/generate", help="LLM service URL")
    parser.add_argument("--host", default="localhost", help="Host to bind services to")
    
    args = parser.parse_args()
    
    # Create service manager
    manager = ServiceManager()
    
    # Update service configurations with command line arguments
    manager.service_configs['stt_server']['args'] = ['--host', args.host, '--port', str(args.stt_port)]
    manager.service_configs['http_api']['args'] = ['--host', args.host, '--port', str(args.http_port), '--llm-url', args.llm_url]
    manager.service_configs['bridge']['args'] = [
        '--host', args.host, '--port', str(args.bridge_port),
        '--stt-url', f'ws://{args.host}:{args.stt_port}',
        '--http-url', f'http://{args.host}:{args.http_port}/transcribe'
    ]
    
    # Setup signal handlers
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)
    signal_handler.service_manager = manager
    
    try:
        # Start services
        if manager.start_all_services(args.include_bridge):
            # Monitor services
            manager.monitor_services()
        else:
            print("❌ Failed to start services")
            sys.exit(1)
            
    except KeyboardInterrupt:
        print("\n🛑 Interrupted by user")
    except Exception as e:
        print(f"❌ Unexpected error: {e}")
    finally:
        manager.stop_all_services()

if __name__ == "__main__":
    main() 