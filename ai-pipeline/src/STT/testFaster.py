import torch
from transformers import AutoModelForSpeechSeq2Seq, AutoProcessor, pipeline
from datasets import load_dataset
from torch.nn.attention import SDPBackend, sdpa_kernel
from tqdm import tqdm
import time
import librosa

# Config
# torch.set_float32_matmul_precision('high')

class asr:
    def __init__(self, modelName: str = 'openai/whisper-large-v3'):
        self.audioStream = []
        self.torch_dtype = torch.float16 if torch.cuda.is_available() else torch.float32
        self.srModel = AutoModelForSpeechSeq2Seq.from_pretrained(modelName, torch_dtype = self.torch_dtype,
                                                            low_cpu_mem_usage = True,
                                                            use_safetensors = True)
        self.processor = AutoProcessor.from_pretrained(modelName)
        self.device = "cuda:0" if torch.cuda.is_available() else "cpu"
    
    def asrPipe(self, dataset, batchSize):
        srModel =self.srModel
        srModel.to(self.device)

        pipe = pipeline("automatic-speech-recognition",
                        model = srModel,
                        tokenizer = self.processor.tokenizer,
                        feature_extractor=self.processor.feature_extractor,
                        torch_dtype=self.torch_dtype,
                        device = self.device,
                        return_timestamps=True)
        result = pipe(dataset,batch_size=batchSize)
        
        print("Transcriptions:\n" + "\n".join([f"  [{i}] {r['text']}" for i, r in enumerate(result)]))
    
    def load_audio(self, file_path, target_sr=16000):
        audio, sr = librosa.load(file_path, sr=target_sr)
        return {"array": audio, "sampling_rate": target_sr}

    def avg_benchmark(pipe, sample, backend, n=5):
        times = []
        for _ in range(n):
            start = time.time()
            pipe(sample.copy())
            times.append(time.time() - start)
        avg = sum(times) / n
        print(f"Avg over {n} runs ({backend.name}): {avg:.2f} seconds")

        
if __name__ == "__main__":
    ASR = asr()
    start = time.perf_counter()
    ASR.asrPipe(["./src/STT/common_voice_th_23646621.mp3","./src/STT/common_voice_th_23646622.mp3"], 2)
    end = time.perf_counter()
    print(f'''Start at [{start}] and End at[{end}]\nTook: [{end - start} seconds] ''')