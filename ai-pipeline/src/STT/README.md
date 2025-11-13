# STT WebSocket System

This directory contains a WebSocket-based Speech-to-Text (STT) system that supports real-time audio streaming and integration with LLM services.

## Architecture

```
Client → WebSocket → STT Server → HTTP API → LLM Service
   ↓         ↓           ↓           ↓          ↓
PCM Audio  WebSocket  Whisper    Flask API   LLM Response
Chunks     Protocol   Model      Server      (Text)
```

### Components

1. **STT WebSocket Server** (`asr_websocket.py`)
   - Receives PCM audio chunks via WebSocket
   - Processes audio using Faster Whisper
   - Returns transcriptions in real-time
   - Supports multiple concurrent clients

2. **HTTP API Server** (`stt_http_api.py`)
   - Receives transcriptions from STT server
   - Forwards text to LLM service via HTTP
   - Returns LLM responses

3. **STT-LLM Bridge** (`stt_bridge.py`)
   - Connects STT WebSocket server to HTTP API
   - Broadcasts conversation results to clients
   - Manages the complete pipeline

4. **Test Client** (`test_websocket_client.py`)
   - Demonstrates how to connect to STT server
   - Sends live audio or audio files
   - Receives transcriptions and responses

## Installation

1. Install dependencies:
```bash
pip install -r requirements.txt
```

2. Ensure you have the required audio libraries:
```bash
# For Windows
pip install pyaudio

# For Linux
sudo apt-get install portaudio19-dev
pip install pyaudio
```

## Usage

### 1. Start the STT WebSocket Server

```bash
python asr_websocket.py --host localhost --port 8765
```

**Options:**
- `--host`: Host to bind to (default: localhost)
- `--port`: Port to bind to (default: 8765)

### 2. Start the HTTP API Server

```bash
python stt_http_api.py --host localhost --port 5000 --llm-url http://localhost:8000/generate
```

**Options:**
- `--host`: Host to bind to (default: localhost)
- `--port`: Port to bind to (default: 5000)
- `--llm-url`: LLM service URL (default: http://localhost:8000/generate)
- `--debug`: Enable debug mode

### 3. Start the STT-LLM Bridge (Optional)

```bash
python stt_bridge.py --host localhost --port 8766 --stt-url ws://localhost:8765 --http-url http://localhost:5000/transcribe
```

**Options:**
- `--host`: Host to bind to (default: localhost)
- `--port`: Port to bind to (default: 8766)
- `--stt-url`: STT WebSocket server URL
- `--http-url`: HTTP API URL

### 4. Test with Client

#### Live Audio Streaming:
```bash
python test_websocket_client.py --server ws://localhost:8765 --mode live
```

#### Audio File Testing:
```bash
python test_websocket_client.py --server ws://localhost:8765 --mode file --audio-file test.wav
```

## API Reference

### STT WebSocket Server Messages

#### Client → Server (Audio)
- **Raw PCM Data**: Send binary PCM audio chunks (16-bit, 16kHz, mono)

#### Client → Server (Control)
```json
{
  "type": "ping"
}
```

```json
{
  "type": "reset"
}
```

```json
{
  "type": "get_status"
}
```

#### Server → Client
```json
{
  "type": "status",
  "message": "Connected to STT server",
  "sample_rate": 16000,
  "chunk_duration_ms": 1000
}
```

```json
{
  "type": "transcription",
  "text": "สวัสดีครับ",
  "confidence": 1.0
}
```

```json
{
  "type": "audio_received",
  "status": "speech_detected"
}
```

### HTTP API Endpoints

#### POST /transcribe
**Request:**
```json
{
  "text": "สวัสดีครับ"
}
```

**Response:**
```json
{
  "status": "success",
  "transcription": "สวัสดีครับ",
  "llm_response": "สวัสดีครับ มีอะไรให้ช่วยเหลือไหมครับ?",
  "timestamp": 1640995200.0
}
```

#### POST /batch_transcribe
**Request:**
```json
{
  "transcriptions": ["สวัสดีครับ", "ขอบคุณครับ"]
}
```

#### GET /health
**Response:**
```json
{
  "status": "healthy",
  "service": "STT-HTTP-API",
  "timestamp": 1640995200.0,
  "stats": {
    "total_requests": 100,
    "successful_requests": 95,
    "failed_requests": 5
  }
}
```

## Audio Format Requirements

- **Sample Rate**: 16,000 Hz
- **Channels**: 1 (Mono)
- **Bit Depth**: 16-bit
- **Format**: PCM (uncompressed)
- **Chunk Size**: 1024 samples (64ms at 16kHz)

## Configuration

### STT Server Parameters

```python
# Audio processing
sample_rate = 16000
chunk_duration_ms = 1000  # 1 second chunks
silence_threshold_ms = 3000  # 3 seconds of silence to end sentence

# Audio quality thresholds
loudness_threshold = 200
rms_threshold = 50
target_rms = 3000

# Whisper model
model_name = "large-v3"
device = "cuda"
compute_type = "int8_float16"
```

### Performance Tuning

1. **Reduce Latency**: Decrease `chunk_duration_ms` and `silence_threshold_ms`
2. **Improve Accuracy**: Use larger Whisper model or increase `beam_size`
3. **Handle Multiple Clients**: Increase server resources and connection limits

## Error Handling

The system includes comprehensive error handling:

- **Connection Errors**: Automatic reconnection attempts
- **Audio Processing Errors**: Graceful degradation with logging
- **LLM Service Errors**: Fallback responses and retry logic
- **Client Disconnections**: Proper cleanup and resource management

## Monitoring

### Logging
All components use structured logging with different levels:
- `INFO`: Normal operation
- `WARNING`: Non-critical issues
- `ERROR`: Critical errors

### Statistics
Each component tracks:
- Request counts
- Success/failure rates
- Processing times
- Client connections

## Troubleshooting

### Common Issues

1. **Audio Not Detected**
   - Check microphone permissions
   - Verify audio format (16kHz, 16-bit, mono)
   - Adjust loudness threshold

2. **High Latency**
   - Reduce chunk duration
   - Use smaller Whisper model
   - Optimize network connection

3. **Poor Transcription Quality**
   - Use larger Whisper model
   - Improve audio quality
   - Adjust audio preprocessing parameters

4. **Connection Issues**
   - Check firewall settings
   - Verify port availability
   - Test network connectivity

### Debug Mode

Enable debug mode for detailed logging:
```bash
python stt_http_api.py --debug
```

## Security Considerations

1. **Authentication**: Implement WebSocket authentication if needed
2. **Rate Limiting**: Add request rate limiting to prevent abuse
3. **Input Validation**: Validate all incoming data
4. **HTTPS/WSS**: Use secure connections in production

## Production Deployment

1. **Use Process Manager**: Use PM2, Supervisor, or systemd
2. **Load Balancing**: Deploy multiple instances behind a load balancer
3. **Monitoring**: Integrate with monitoring systems (Prometheus, Grafana)
4. **Logging**: Use structured logging with log aggregation
5. **SSL/TLS**: Enable secure connections
6. **Resource Limits**: Set appropriate memory and CPU limits

## Example Integration

### JavaScript Client
```javascript
const ws = new WebSocket('ws://localhost:8765');

ws.onopen = () => {
    console.log('Connected to STT server');
};

ws.onmessage = (event) => {
    const data = JSON.parse(event.data);
    if (data.type === 'transcription') {
        console.log('Transcription:', data.text);
    }
};

// Send audio chunk
function sendAudioChunk(audioData) {
    ws.send(audioData);
}
```

### Python Client
```python
import asyncio
import websockets
import pyaudio

async def send_audio():
    async with websockets.connect('ws://localhost:8765') as websocket:
        # Setup audio stream
        p = pyaudio.PyAudio()
        stream = p.open(format=pyaudio.paInt16, channels=1, rate=16000, input=True, frames_per_buffer=1024)
        
        while True:
            audio_chunk = stream.read(1024)
            await websocket.send(audio_chunk)
            
            # Handle responses
            try:
                response = await asyncio.wait_for(websocket.recv(), timeout=0.1)
                data = json.loads(response)
                if data['type'] == 'transcription':
                    print(f"Transcription: {data['text']}")
            except asyncio.TimeoutError:
                pass
```

## License

This project is part of the Project-Buddy AI pipeline system. 