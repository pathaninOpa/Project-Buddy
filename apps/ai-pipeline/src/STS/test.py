import pyaudio
import collections
import time
import threading
from pipeline import RUN 
import webrtcvad

FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 16000
FRAME_DURATION = 30
FRAME_SIZE = int(RATE * FRAME_DURATION / 1000)
BUFFER_DURATION = 1
MAX_BUFFER_FRAMES = int(BUFFER_DURATION * 1000 / FRAME_DURATION)

class AudioStreamer:
    def __init__(self):
        self.vad = webrtcvad.Vad(2)
        self.audio_interface = pyaudio.PyAudio()
        self.stream = self.audio_interface.open(
            format=FORMAT,
            channels=CHANNELS,
            rate=RATE,
            input=True,
            frames_per_buffer=FRAME_SIZE
        )
        self.run_pipeline = RUN()

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

    def run(self):
        try:
            for speech in self.detect_speech():
                result = self.run_pipeline.pipeline(speech)
                if result:
                    self.play_audio(result)
        except KeyboardInterrupt:
            print("\n[INFO] Exiting.")
        finally:
            self.stream.stop_stream()
            self.stream.close()
            self.audio_interface.terminate()

if __name__ == "__main__":
    listener = AudioStreamer()
    listener.run()
