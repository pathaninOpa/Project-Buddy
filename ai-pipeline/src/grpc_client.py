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
MIIFLjCCAxagAwIBAgIUdeQZg1IFOgh+gcHm6/rCiFUEK9gwDQYJKoZIhvcNAQEL
BQAwGTEXMBUGA1UEAwwOYXBpLmJ1ZGR5LnJlc3QwHhcNMjUxMTEzMDI0MjM3WhcN
MjYxMTEzMDI0MjM3WjAZMRcwFQYDVQQDDA5hcGkuYnVkZHkucmVzdDCCAiIwDQYJ
KoZIhvcNAQEBBQADggIPADCCAgoCggIBALdBq4Pcs6pBO9fWZ08Tj8hI2gMjmlfZ
tNhEVuleUIyqSMfZ21cdOjrNqyP0d8ajp1qRVZ0YYhXKfy9ZZ/WyJIC5ZdeKCW8s
mlBCuZvIkUpVcxTURZr+NyesB4gqWDQUFG4MUWsiUk3KO3/1wiN5IhyjX/ApJPLd
+CIMnrMIaqXUfZO344vGQQfKgzBY0jxOQhINoXVYZcSxzDJhV0Zd+13PDmJRAMYb
IoaiZRklsvcyx0B4nE4PKdeHHcBNKULpH5AYu2ZH+rT3AqCZMxDB79740Ykt42/N
RS6tQr8w6OljHM0+/Nt98cn+JSyikuyrOX//mVfD8zpvt6+CufsblZHIJxOWRBea
0C+IG0/4aDamEQkanedNvw/oDwQBlmw8ie1ezaG8C30++tMrxZSZ+G+DACc622Dj
pNqS93eTsbHT8S25M0tU27RLeqZcydhI0HCoNXq4diG/EX3jbyJaI3gvQvwDQVSU
1JKXosKBmbsaaqUy759aEU5p8sRCASXLS2nBM8P5/s3AjMaO5tGkt+ti/lTpe/gq
C1QToWwRfJiriscpy/9wVFfzSitPwPmB3skLvB6uFwFmvtcOycLZaXu9qfiG3h6N
sdgIj49G8OQe8Lq+eEIpLTmybmjciAr7T85R7yZtT7Lt/fO9naVR/3GILOtK4R/2
2Id2U4vnIyi7AgMBAAGjbjBsMB0GA1UdDgQWBBSGpDPyAv+HD49S7UeN95PocTSh
ZDAfBgNVHSMEGDAWgBSGpDPyAv+HD49S7UeN95PocTShZDAPBgNVHRMBAf8EBTAD
AQH/MBkGA1UdEQQSMBCCDmFwaS5idWRkeS5yZXN0MA0GCSqGSIb3DQEBCwUAA4IC
AQBbY7ePfZKqHp37vyYkaTWuaxwuhnwhkJymWuSgjbeMT5Nqm4kIf5YJLx6yVPrW
ay6tOWhpD3CLx8826VKvJ15iwj4d78Q0p/J9HF5+67zcFSlkRpH27i/1pdZ1Xx4m
tWAAGyzuhs3TW6xIzTvRbhymElU0M4aTkJJP83CtJDswTAzBkhnqLPAyBJM9bXpf
6O4y3DvNlQaA/te20RoGAnPAkdyCmnhuPBRTMCHd+zcUIsz1ut4qlLfbmdHBQQyG
CZFX2JPuetPhAZtbKKYtOhF0nAiMO/k35sfEB6ucxuRJR2TqR2CXieXZvGaEVwG/
7g3vhavuOhfowX+pQmU+JgMZrqvzqXFfWl5hCxVO4VXZWx93SrxSr6SF7Apa8JPI
uMWO/dDZOtW8WBzjkV8tk6sQUIQMjjJgx0vPkEKjQNlgJdvF3nNKrH2VwzYcXAJo
bVsA7OJ20EYqG2of+HXcnmaWUJrkFf9rQjrgjYO64CAYDUCSTMXM8fkKxRxIuHba
xvtJsr8tZg1neB9HrdfPHJbImSUiezMQKrplqthl7ty5t7di0Erw+Q34pmuT0Ky4
6iO1whAy94EVElpfCkEf9zV+oFaxR7mX/A19hgBLiONKI3HuSFCk+tDvyRcGMdRE
o8qBuh3U6r8u0DVPSIJdr3uy/cGzl8jTsU4DYwJLTCK7Rw==
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
    # channel = grpc.insecure_channel('localhost:50051')
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