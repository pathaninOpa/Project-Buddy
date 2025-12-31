import sys
import os
import logging
import torch
import soundfile as sf
import pandas as pd
from tqdm import tqdm
from datasets import load_dataset
from faster_whisper import WhisperModel
import traceback

# Ensure src is in path
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
from evaluation.metrics import calculate_wer, calculate_cer

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

def run_evaluation(num_samples=50):
    """
    Downloads a subset of the Thai Elderly Speech dataset and evaluates it.
    """
    dataset = None
    
    # Attempt to load without streaming (will download the whole config split)
    try:
        config_name = "thai_elderly_speech_healthcare_seacrowd_sptext"
        logging.info(f"Attempting to load dataset '{config_name}' without streaming. This will download the full split.")
        # Using 'train' split for evaluation as per previous attempts. Change to 'test' if available and preferred.
        dataset = load_dataset("SEACrowd/thai_elderly_speech", config_name, split="train", trust_remote_code=True)
    except Exception as e:
        logging.error(f"Failed to load dataset without streaming: {e}")
        logging.error(traceback.format_exc())
        return

    if not dataset:
        logging.error("Could not load dataset. Aborting.")
        return

    logging.info(f"Dataset loaded. It contains {len(dataset)} samples. Limiting to {num_samples} for evaluation.")

    logging.info(f"Loading Whisper model (large-v3)...")
    try:
        device = "cuda" if torch.cuda.is_available() else "cpu"
        compute_type = "int8_float16" if device == "cuda" else "int8"
        model = WhisperModel("large-v3", device=device, compute_type=compute_type)
    except Exception as e:
        logging.error(f"Failed to load Whisper model: {e}")
        return

    results = []
    total_wer = 0.0
    total_cer = 0.0
    count = 0

    logging.info(f"Processing {min(num_samples, len(dataset))} samples...")
    
    temp_dir = os.path.join(os.getcwd(), "temp_audio_eval")
    os.makedirs(temp_dir, exist_ok=True)

    # Iterate through a slice of the dataset
    for i in tqdm(range(min(num_samples, len(dataset)))):
        try:
            sample = dataset[i]
        except Exception as e:
            logging.warning(f"Error fetching sample {i}: {e}")
            logging.error(traceback.format_exc())
            continue
            
        audio_data = None
        sample_rate = 16000
        transcript = ""
        
        # Expected structure for seacrowd_sptext: {'id': ..., 'audio': {'path': ..., 'array': ..., 'sampling_rate': ...}, 'text': ...}
        if 'audio' in sample and isinstance(sample['audio'], dict):
            audio_data = sample['audio'].get('array')
            sample_rate = sample['audio'].get('sampling_rate', 16000)
        
        if 'text' in sample:
            transcript = sample['text']
            
        if audio_data is None or not transcript:
            logging.warning(f"Skipping sample {i} due to missing audio or transcript.")
            continue
        
        # Save to temp wav file
        temp_path = os.path.join(temp_dir, f"sample_{i}.wav")
        try:
            sf.write(temp_path, audio_data, sample_rate)
        except Exception as e:
            logging.error(f"Error writing audio for sample {i}: {e}")
            logging.error(traceback.format_exc())
            continue
        
        # Transcribe
        try:
            segments, info = model.transcribe(temp_path, language="th")
            hypothesis = " ".join([s.text for s in segments]).strip()
        except Exception as e:
            logging.error(f"Transcription error for sample {i}: {e}")
            logging.error(traceback.format_exc())
            continue
            
        wer = calculate_wer(transcript, hypothesis)
        cer = calculate_cer(transcript, hypothesis)
        
        total_wer += wer
        total_cer += cer
        count += 1
        
        results.append({
            "reference": transcript,
            "hypothesis": hypothesis,
            "wer": wer,
            "cer": cer
        })
        
        # Clean up
        try:
            os.remove(temp_path)
        except:
            pass

    if count == 0:
        logging.error("No samples processed.")
        return

    avg_wer = (total_wer / count) * 100
    avg_cer = (total_cer / count) * 100
    
    # Clean up temp_dir and its contents
    try:
        for f in os.listdir(temp_dir):
            os.remove(os.path.join(temp_dir, f))
        os.rmdir(temp_dir)
    except Exception as e:
        logging.warning(f"Error cleaning up temporary directory: {e}")

    print("\n" + "="*30)
    print("REAL EVALUATION RESULTS (Subset)")
    print("="*30)
    print(f"Dataset: SEACrowd/thai_elderly_speech ({config_name}) - {count} samples processed")
    print(f"WER: {avg_wer:.2f}%")
    print(f"CER: {avg_cer:.2f}%")
    print("="*30)

    # Save to CSV
    pd.DataFrame(results).to_csv("real_eval_results.csv", index=False)
    
    # Update the summary file
    with open("thesis_results.txt", "a") as f:
        f.write("\n\n[UPDATED REAL EVALUATION]\n")
        f.write(f"Dataset: SEACrowd/thai_elderly_speech ({config_name}) (Subset of {count})\n")
        f.write(f"WER: {avg_wer:.2f}%")
        f.write(f"CER: {avg_cer:.2f}%")

if __name__ == "__main__":
    run_evaluation(50)