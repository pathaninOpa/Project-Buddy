import pyaudio
import collections
import time
import threading
from pipeline import RUN 
import webrtcvad
import subprocess
from io import BytesIO
import datetime

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
        
        # List all available devices
        print("\nAvailable audio devices:")
        for i in range(self.audio_interface.get_device_count()):
            device_info = self.audio_interface.get_device_info_by_index(i)
            print(f"Device {i}: {device_info.get('name')}")
            print(f"  Max Input Channels: {device_info.get('maxInputChannels')}")
            print(f"  Max Output Channels: {device_info.get('maxOutputChannels')}")
            
        # Try to find a working input device
        input_device = None
        for i in range(self.audio_interface.get_device_count()):
            device_info = self.audio_interface.get_device_info_by_index(i)
            try:
                if device_info.get('maxInputChannels') > 0:
                    # Try to open the device briefly to test it
                    test_stream = self.audio_interface.open(
                        format=FORMAT,
                        channels=CHANNELS,
                        rate=RATE,
                        input=True,
                        input_device_index=i,
                        frames_per_buffer=FRAME_SIZE,
                        start=False
                    )
                    test_stream.close()
                    input_device = i
                    print(f"\nUsing audio input device: {device_info.get('name')}")
                    break
            except Exception as e:
                print(f"Device {i} test failed: {str(e)}")
                continue
        
        if input_device is None:
            raise RuntimeError("No input audio devices found")
            
        self.stream = self.audio_interface.open(
            format=FORMAT,
            channels=CHANNELS,
            rate=RATE,
            input=True,
            input_device_index=input_device,
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

    def save_audio_to_mp3(self, audio_bytes: bytes, filename="output.mp3", sample_rate=24000):
        if isinstance(filename, bytes):
            filename = filename.decode("utf-8", errors="replace")
        cmd = [
            "ffmpeg",
            "-y",
            "-f", "s16le",
            "-ar", str(sample_rate),
            "-ac", "1",
            "-i", "pipe:0",
            filename
        ]
        try:
            process = subprocess.Popen(
                cmd,
                stdin=subprocess.PIPE,
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL,
                shell=False
            )
            process.communicate(input=audio_bytes)
            print(f"[INFO] Saved to {filename}")
        except Exception as e:
            print(f"[ERROR] Failed to save MP3: {e}")

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
                    self.save_audio_to_mp3(bytes(result))
                    print("[INFO] Playing response...")
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
