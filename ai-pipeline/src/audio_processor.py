import numpy as np
import sounddevice as sd
import webrtcvad
import io
import soundfile as sf
from scipy.signal import butter, lfilter

class AudioProcessor:
    def __init__(self):
        self.vad = webrtcvad.Vad(1)
        self.sample_rate = 16000
        self.channels = 1
        self.dtype = np.int16

    def is_loud_enough(self, audio_np, threshold=200) -> bool:
        rms = np.sqrt(np.mean(audio_np.astype(np.float32)**2))
        return rms > threshold

    def has_speech(self, audio_bytes, sample_rate=16000) -> bool:
        frame_duration_ms = 10
        bytes_per_sample = 2
        frame_size = int(sample_rate * (frame_duration_ms / 1000.0)) * bytes_per_sample
        for i in range(0, len(audio_bytes) - frame_size, frame_size):
            frame = audio_bytes[i:i + frame_size]
            if self.vad.is_speech(frame, sample_rate):
                return True
        return False

    def normalize_audio(self, audio_np, target_rms=3000):
        rms = np.sqrt(np.mean(audio_np.astype(np.float32)**2))
        if rms == 0:
            return audio_np
        gain = target_rms / rms
        normalized = audio_np.astype(np.float32) * gain
        return np.clip(normalized, -32768, 32767).astype(np.int16)

    def highpass_filter(self, audio_np, sr=16000, cutoff=150):
        b, a = butter(1, cutoff / (0.5 * sr), btype='high')
        filtered = lfilter(b, a, audio_np.astype(np.float32))
        return filtered.astype(np.int16)

    def record_audio(self, duration=1.0):
        """Record audio from microphone with preprocessing."""
        frames = int(self.sample_rate * duration)
        audio_data = sd.rec(
            frames,
            samplerate=self.sample_rate,
            channels=self.channels,
            dtype=self.dtype,
            blocking=True
        )
        
        audio_bytes = audio_data.tobytes()
        audio_np = np.frombuffer(audio_bytes, dtype=np.int16)
        audio_np = self.highpass_filter(audio_np)
        if not self.is_loud_enough(audio_np, threshold=50):
            return None
        if not self.has_speech(audio_bytes, self.sample_rate):
            return None

        audio_np = self.normalize_audio(audio_np)
        float_audio = audio_np.astype(np.float32) / 32768.0
        
        buffer = io.BytesIO()
        sf.write(buffer, float_audio, self.sample_rate, format='MP3')
        buffer.seek(0)
        return buffer.read()

    def preprocess_audio(self, audio_bytes):
        """Preprocess existing audio bytes."""
        audio_np = np.frombuffer(audio_bytes, dtype=np.int16)
        audio_np = self.highpass_filter(audio_np)
        
        if not self.is_loud_enough(audio_np, threshold=50):
            return b""
            
        if not self.has_speech(audio_bytes, self.sample_rate):
            return b""
            
        audio_np = self.normalize_audio(audio_np)
        float_audio = audio_np.astype(np.float32) / 32768.0
        
        buffer = io.BytesIO()
        sf.write(buffer, float_audio, self.sample_rate, format='RAW', subtype='PCM_16')
        buffer.seek(0)
        
        return buffer.read()
