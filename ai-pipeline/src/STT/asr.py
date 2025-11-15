from faster_whisper import WhisperModel
import pyaudio
import numpy as np
import webrtcvad
import io
import soundfile as sf
import re
from colorama import Fore, Style, init
import wave
from scipy.signal import butter, lfilter  

init(autoreset=True)
NEON_GREEN = Fore.GREEN
RESET_COLOR = Style.RESET_ALL

vad = webrtcvad.Vad(1)
model = WhisperModel("large-v3", device="cuda", compute_type="int8_float16")

def is_loud_enough(audio_np, threshold=200):
    rms = np.sqrt(np.mean(audio_np.astype(np.float32)**2))
    return rms > threshold

def has_speech(audio_bytes, sample_rate=16000):
    frame_duration_ms = 10
    bytes_per_sample = 2
    frame_size = int(sample_rate * (frame_duration_ms / 1000.0)) * bytes_per_sample
    for i in range(0, len(audio_bytes) - frame_size, frame_size):
        frame = audio_bytes[i:i + frame_size]
        if vad.is_speech(frame, sample_rate):
            return True
    return False

def is_mostly_thai(text, threshold=0.5):
    thai_chars = re.findall(r'[\u0E00-\u0E7F]', text)
    return len(thai_chars) / max(len(text), 1) > threshold

def normalize_audio(audio_np, target_rms=3000):
    rms = np.sqrt(np.mean(audio_np.astype(np.float32)**2))
    if rms == 0:
        return audio_np
    gain = target_rms / rms
    normalized = audio_np.astype(np.float32) * gain
    return np.clip(normalized, -32768, 32767).astype(np.int16)

def highpass_filter(audio_np, sr=16000, cutoff=150):
    b, a = butter(1, cutoff / (0.5 * sr), btype='high')
    filtered = lfilter(b, a, audio_np.astype(np.float32))
    return filtered.astype(np.int16)

def record_stream_chunk(stream, chunk_seconds=1.0):
    frames = []
    for _ in range(int(16000 / 1024 * chunk_seconds)):
        data = stream.read(1024, exception_on_overflow=False)
        frames.append(data)
    return b''.join(frames)

def save_wav(filename, audio_bytes, sample_rate=16000):
    with wave.open(filename, 'wb') as wf:
        wf.setnchannels(1)
        wf.setsampwidth(2)  # 2 bytes for paInt16
        wf.setframerate(sample_rate)
        wf.writeframes(audio_bytes)

# -------------------------------
# Transcription Pipeline
# -------------------------------

def transcribe_audio_bytes(audio_bytes):
    audio_np = np.frombuffer(audio_bytes, dtype=np.int16)

    audio_np = highpass_filter(audio_np)

    if np.sqrt(np.mean(audio_np.astype(np.float32)**2)) < 50:
        return ""

    audio_np = normalize_audio(audio_np)
    float_audio = audio_np.astype(np.float32) / 32768.0

    buffer = io.BytesIO()
    sf.write(buffer, float_audio, 16000, format='MP3')
    buffer.seek(0)

    segments, _ = model.transcribe(buffer, language="th", beam_size=1)
    transcript = ' '.join(segment.text for segment in segments).strip()

    if not transcript or not is_mostly_thai(transcript):
        return ""

    return transcript

# -------------------------------
# Main Audio Loop
# -------------------------------

def main():
    p = pyaudio.PyAudio()
    stream = p.open(format=pyaudio.paInt16, channels=1, rate=16000, input=True, frames_per_buffer=1024)

    print("Listening (Thai-only, sentence ends after 4s of silence)... Press Ctrl+C to stop.")

    sentence_buffer = bytearray()
    recorded_audio = bytearray()
    silence_duration = 0.0
    chunk_duration = 1.0  # seconds

    try:
        while True:
            chunk = record_stream_chunk(stream, chunk_seconds=chunk_duration)
            audio_np = np.frombuffer(chunk, dtype=np.int16)

            if is_loud_enough(audio_np) and has_speech(chunk):
                # Reset silence tracker
                silence_duration = 0.0
                sentence_buffer.extend(chunk)
                recorded_audio.extend(chunk)
            else:
                silence_duration += chunk_duration

                if silence_duration >= 3.0 and sentence_buffer:
                    # Transcribe and reset
                    transcript = transcribe_audio_bytes(sentence_buffer)
                    if transcript:
                        print(NEON_GREEN + transcript + RESET_COLOR)
                    sentence_buffer = bytearray()  # reset for next sentence

    except KeyboardInterrupt:
        print("\nStopping...")

    finally:
        # Flush final sentence if any
        if sentence_buffer:
            transcript = transcribe_audio_bytes(sentence_buffer)
            if transcript:
                print(NEON_GREEN + transcript + RESET_COLOR)

        if recorded_audio:
            print(f"Saving continuous recorded audio ({len(recorded_audio)} bytes) to test.wav")
            save_wav("test.wav", recorded_audio)
        else:
            print("No audio to save.")

        stream.stop_stream()
        stream.close()
        p.terminate()



if __name__ == "__main__":
    main()
