from google.cloud import texttospeech
client = texttospeech.TextToSpeechClient()
inputText = texttospeech.SynthesisInput(text="สวัสดีครับ")
voice = texttospeech.VoiceSelectionParams(
    language_code="th-TH",
    name="th-TH-Chirp3-HD-Charon",
    ssml_gender=texttospeech.SsmlVoiceGender.MALE
)
audio_config = texttospeech.AudioConfig(audio_encoding=texttospeech.AudioEncoding.MP3)
response = client.synthesize_speech(input=inputText,voice=voice, audio_config= audio_config)
with open ("output.mp3", "wb") as out:
    out.write(response.audio_content)
    print("Audio content written to \"output.mp3\"") 