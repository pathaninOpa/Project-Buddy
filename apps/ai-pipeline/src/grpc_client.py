import grpc
from protos import speech_service_pb2
from protos import speech_service_pb2_grpc
import pyaudio
import webrtcvad
import collections
import time
from audio_processor import AudioProcessor

FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
FRAME_DURATION = 30  # ms
FRAME_SIZE = int(RATE * FRAME_DURATION / 1000)
BUFFER_DURATION = 1  # seconds
MAX_BUFFER_FRAMES = int(BUFFER_DURATION * 1000 / FRAME_DURATION)

class AudioStreamer:
    def __init__(self):
        self.vad = webrtcvad.Vad(1)
        self.audio_interface = pyaudio.PyAudio()
        self.stream = self.audio_interface.open(
            format=FORMAT,
            channels=CHANNELS,
            rate=RATE,
            input=True,
            frames_per_buffer=FRAME_SIZE
        )
        self.processor = AudioProcessor()

    def read_audio_frame(self):
        return self.stream.read(FRAME_SIZE, exception_on_overflow=False)

    def detect_speech(self):
        ring_buffer = collections.deque(maxlen=MAX_BUFFER_FRAMES)
        triggered = False
        voiced_frames = []

        print("🎙️ Listening... Say 'Buddy' or 'บัดดี้' to activate...")

        while True:
            frame = self.read_audio_frame()
            is_speech = self.vad.is_speech(frame, RATE)

            if not triggered:
                ring_buffer.append((frame, is_speech))
                num_voiced = len([f for f, speech in ring_buffer if speech])
                if num_voiced > 0.8 * ring_buffer.maxlen:
                    triggered = True
                    print("[VAD] Speech started.")
                    voiced_frames.extend([f for f, s in ring_buffer])
                    ring_buffer.clear()
            else:
                voiced_frames.append(frame)
                ring_buffer.append((frame, is_speech))
                num_unvoiced = len([f for f, speech in ring_buffer if not speech])
                if num_unvoiced > 0.8 * ring_buffer.maxlen:
                    print("[VAD] Speech ended. Processing...")
                    yield b"".join(voiced_frames)
                    triggered = False
                    ring_buffer.clear()
                    voiced_frames = []

    def play_audio(self, audio_bytes):
        if not audio_bytes:
            return
        try:
            stream = self.audio_interface.open(
                format=pyaudio.paInt16,
                channels=1,
                rate=24000,
                output=True
            )
            stream.write(bytes(audio_bytes))
            stream.stop_stream()
            stream.close()
        except Exception as e:
            print(f"[Playback Error] {e}")

def run_speech_pipeline(audio_data):
    # init gRPC channel
    channel = grpc.insecure_channel('localhost:50051')
    stub = speech_service_pb2_grpc.SpeechServiceStub(channel)

    request = speech_service_pb2.AudioRequest(
        audio_data=audio_data,
        sample_rate=16000
    )

    try:
        response = stub.ProcessSpeech(request)
        return response.audio_data
    except grpc.RpcError as e:
        print(f"RPC failed: {str(e)}")
        return b""

if __name__ == '__main__':
    streamer = AudioStreamer()
    try:
        for speech in streamer.detect_speech():
            processed = streamer.processor.preprocess_audio(speech)
            if not processed:
                print("No valid speech detected, try again...")
                continue
            print("[INFO] Sending audio to server...")
            response_audio = run_speech_pipeline(processed)
            if response_audio:
                print("[INFO] Playing response...")
                streamer.play_audio(response_audio)
            else:
                print("[INFO] No response audio received.")
    except KeyboardInterrupt:
        print("\nExiting...")
    finally:
        streamer.stream.stop_stream()
        streamer.stream.close()
        streamer.audio_interface.terminate()