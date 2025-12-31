import sys
import os
import time
import json
import logging
import argparse
import pandas as pd
from tqdm import tqdm
from faster_whisper import WhisperModel

# Ensure src is in path
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
from evaluation.metrics import calculate_wer, calculate_cer

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')

def evaluate_stt(dataset_path, model_size="large-v3", device="cuda", compute_type="int8_float16"):
    """
    Evaluates STT performance on a dataset.
    """
    logging.info(f"Loading model: {model_size} on {device} ({compute_type})")
    try:
        model = WhisperModel(model_size, device=device, compute_type=compute_type)
    except Exception as e:
        logging.error(f"Failed to load model: {e}")
        return

    # Load dataset
    try:
        if dataset_path.endswith('.csv'):
            df = pd.read_csv(dataset_path)
        elif dataset_path.endswith('.json'):
            df = pd.read_json(dataset_path)
        else:
            logging.error("Unsupported file format. Use CSV or JSON.")
            return
    except Exception as e:
        logging.error(f"Failed to load dataset: {e}")
        return

    # Normalize column names
    if 'path' in df.columns: df.rename(columns={'path': 'audio_path'}, inplace=True)
    if 'sentence' in df.columns: df.rename(columns={'sentence': 'transcript'}, inplace=True)
    if 'text' in df.columns: df.rename(columns={'text': 'transcript'}, inplace=True)

    if 'audio_path' not in df.columns or 'transcript' not in df.columns:
        logging.error(f"Dataset must contain 'audio_path' and 'transcript' columns. Found: {df.columns}")
        return

    results = []
    total_wer = 0.0
    total_cer = 0.0
    total_rtf = 0.0
    count = 0

    logging.info(f"Starting evaluation on {len(df)} samples...")

    for index, row in tqdm(df.iterrows(), total=len(df)):
        audio_path = row['audio_path']
        reference = str(row['transcript'])

        if not os.path.exists(audio_path):
            # Try relative path
            if os.path.exists(os.path.join(os.path.dirname(dataset_path), audio_path)):
                audio_path = os.path.join(os.path.dirname(dataset_path), audio_path)
            else:
                logging.warning(f"File not found: {audio_path}")
                continue

        start_time = time.time()
        try:
            segments, info = model.transcribe(audio_path, language="th")
            hypothesis = " ".join([s.text for s in segments]).strip()
            process_time = time.time() - start_time
            
            audio_duration = info.duration
            rtf = process_time / audio_duration if audio_duration > 0 else 0

        except Exception as e:
            logging.error(f"Error processing {audio_path}: {e}")
            continue

        wer = calculate_wer(reference, hypothesis)
        cer = calculate_cer(reference, hypothesis)

        total_wer += wer
        total_cer += cer
        total_rtf += rtf
        count += 1

        results.append({
            "audio_path": audio_path,
            "reference": reference,
            "hypothesis": hypothesis,
            "wer": wer,
            "cer": cer,
            "rtf": rtf
        })

    if count == 0:
        logging.error("No samples processed.")
        return

    avg_wer = (total_wer / count) * 100
    avg_cer = (total_cer / count) * 100
    avg_rtf = total_rtf / count

    print("\n" + "="*30)
    print("STT EVALUATION RESULTS")
    print("="*30)
    print(f"Samples: {count}")
    print(f"WER: {avg_wer:.2f}%")
    print(f"CER: {avg_cer:.2f}%")
    print(f"RTF: {avg_rtf:.2f}")
    print("="*30)

    output_csv = "stt_evaluation_results.csv"
    pd.DataFrame(results).to_csv(output_csv, index=False)
    logging.info(f"Detailed results saved to {output_csv}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Evaluate STT Performance")
    parser.add_argument("dataset_path", help="Path to CSV/JSON dataset")
    parser.add_argument("--model", default="large-v3", help="Whisper model size")
    parser.add_argument("--device", default="cuda", help="Device (cuda/cpu)")
    args = parser.parse_args()

    evaluate_stt(args.dataset_path, args.model, args.device)
