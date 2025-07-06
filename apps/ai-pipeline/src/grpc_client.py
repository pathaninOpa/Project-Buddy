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
FRAME_DURATION = 30
FRAME_SIZE = int(RATE * FRAME_DURATION / 1000)
BUFFER_DURATION = 1
MAX_BUFFER_FRAMES = int(BUFFER_DURATION * 1000 / FRAME_DURATION)
server_crt = """-----BEGIN CERTIFICATE-----
MIIDLjCCAhagAwIBAgIUTSD75uKATgqqm8vODodKPlb7jkcwDQYJKoZIhvcNAQEL
BQAwGTEXMBUGA1UEAwwOYXBpLmJ1ZGR5LnJlc3QwHhcNMjUwNzA2MTQ1ODU0WhcN
MjYwNzA2MTQ1ODU0WjAZMRcwFQYDVQQDDA5hcGkuYnVkZHkucmVzdDCCASIwDQYJ
KoZIhvcNAQEBBQADggEPADCCAQoCggEBAJQH2ycwogpVfSVO35RYo3AHii2B/ZFG
T9q5RkyJNUdTJp93yaZ6zQdZbVgN19/XsA9o8yhjtFnPSSvTCR+eVK9fpSW7fbFf
dvzflcsMolwHK4I7Daujv8zBjuEd5epERgeb1RjziHCQlNzfHINeQ50SRO8ABQwz
mWQupE8Cs3a2RmasQdEBSaua/AUEtrDvfW2qqANkTAZJmira9h0IQ+dScrnwT+do
HmPkEdlD/iCb9qi6DGmbZK8fDbX5JftK9KqDeOZUoFMuz+6nEN3CtPcWdSjx5sGz
layiO+7bW+OC1tVx+7pfV3kYpO5YzNunnt7chDvoFgg2mXQJCe7VmJsCAwEAAaNu
MGwwHQYDVR0OBBYEFC5sGc1VHa9bkmavMWJ3IUrO9wtbMB8GA1UdIwQYMBaAFC5s
Gc1VHa9bkmavMWJ3IUrO9wtbMA8GA1UdEwEB/wQFMAMBAf8wGQYDVR0RBBIwEIIO
YXBpLmJ1ZGR5LnJlc3QwDQYJKoZIhvcNAQELBQADggEBACDPozL2wud8sHNRyFXz
vbPf1nnTS7Mf8m6jKRE+FCbnq1AHe5jne7E7GKAz2FlsZZEGM2lwuwHcMsiG1m3f
ZNVhiVkYh5En4UvasnncTo2m1e02fxz8olp7WRWDJOLPGkEPuxQLCLpwSUYCyr5o
nDdLKpLEEOwe/Rnzb+7c2DbPmmHyQ6muVoL0b5xBHyJRJk3Porz3NwpwmI9TR6Yp
b+ItVimNRON6hullPxvEqH1nNWbgjBlCzXXxugH7MEhzwYME6o++XSPkPRIVCc4X
jkaTyKE2sR9kyPXgTLfPu3Du21ybvQLMy/z7mUPGQXmKIiYYLZ/d1XiNtcgfHlWL
ihc=
-----END CERTIFICATE-----"""

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
    trusted_certs = server_crt.encode("utf-8")
    credentials = grpc.ssl_channel_credentials(root_certificates=trusted_certs)

    channel = grpc.secure_channel('api.buddy.rest:50051', credentials)
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