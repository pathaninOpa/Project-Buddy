import torch
from transformers import AutoModelForSpeechSeq2Seq, AutoProcessor, pipeline
from datasets import load_dataset
from torch.nn.attention import SDPBackend, sdpa_kernel
from tqdm import tqdm
import time

# Config
torch.set_float32_matmul_precision('high')

class asr:
    def __init__(self, modelName: str = 'openai/whisper-large-v3'):
        self.audioStream = []
        self.torch_dtype = torch.float16 if torch.cuda.is_available() else torch.float32
        self.srModel = AutoModelForSpeechSeq2Seq.from_pretrained(modelName, torch_dtype = self.torch_dtype,
                                                            low_cpu_mem_usage = True,
                                                            use_safetensors = True)
        self.processor = AutoProcessor.from_pretrained(modelName)
        self.device = "cuda:0" if torch.cuda.is_available() else "cpu"
    
    def asrPipe(self, dataset):
        srModel =self.srModel
        srModel.to(self.device)
        srModel.generation_config.cache_implementation = "static"
        srModel.generation_config.max_new_tokens = 256
        srModel.forward = torch.compile(srModel.forward, mode="reduce-overhead", fullgraph=True)

        pipe = pipeline("automatic-speech-recognition",
                        model = srModel,
                        tokenizer = self.processor.tokenizer,
                        feature_extractor=self.processor.feature_extractor,
                        torch_dtype=self.torch_dtype,
                        device = self.device,
                        return_timestamps=True)
        sample = dataset[0]["audio"]    
        # warm-up 2 steps
        for _ in tqdm(range(2), desc="Warm-up step"): 
            with sdpa_kernel(SDPBackend.FLASH_ATTENTION):
                result = pipe(sample.copy(), generate_kwargs={"min_new_tokens": 256, "max_new_tokens": 256})

        # fast run
        with sdpa_kernel(SDPBackend.FLASH_ATTENTION):
            result = pipe(sample.copy())
        
        print("Transcription:\n ",result["text"])

    def avg_benchmark(pipe, sample, backend, n=5):
        times = []
        for _ in range(n):
            with sdpa_kernel(backend):
                start = time.time()
                pipe(sample.copy())
                times.append(time.time() - start)
        avg = sum(times) / n
        print(f"Avg over {n} runs ({backend.name}): {avg:.2f} seconds")

        
if __name__ == "__main__":
    ASR = asr()
    dataset = load_dataset("distil-whisper/librispeech_long", "clean", split="validation")
    start = time.perf_counter()
    ASR.asrPipe(dataset)
    end = time.perf_counter()
    print(f'''Start at [{start}] and End at[{end}]\nTook: [{end - start} seconds] ''')