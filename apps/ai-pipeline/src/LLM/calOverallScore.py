import json
import re
from pathlib import Path
from collections import defaultdict

# Setup
script_dir = Path(__file__).parent.resolve()
output_dir = script_dir / "OUT"
output_dir.mkdir(parents=True, exist_ok=True)
json_file = output_dir / "llm_eval_batch.json"

# Load data
with open(json_file, encoding='utf-8') as f:
    data = json.load(f)

# Score containers grouped by category
category_scores = defaultdict(lambda: {"model_a": [], "model_b": []})

# Iterate and extract scores
for entry in data:
    evaluation = entry.get("evaluation", "")
    prompt = entry.get("prompt", "")
    category = entry.get("category", "Unknown")

    try:
        a_score = None
        b_score = None
        for line in evaluation.splitlines():
            if "คะแนน A" in line:
                # Get the number after the last colon
                parts = line.split(":")
                if len(parts) > 1:
                    num_part = parts[-1].strip()
                    num_match = re.match(r"([0-9]+(?:\.[0-9]+)?)", num_part)
                    if num_match:
                        a_score = float(num_match.group(1))
            if "คะแนน B" in line:
                parts = line.split(":")
                if len(parts) > 1:
                    num_part = parts[-1].strip()
                    num_match = re.match(r"([0-9]+(?:\.[0-9]+)?)", num_part)
                    if num_match:
                        b_score = float(num_match.group(1))
        if a_score is not None and b_score is not None:
            if 0 <= a_score <= 10 and 0 <= b_score <= 10:
                category_scores[category]["model_a"].append(a_score)
                category_scores[category]["model_b"].append(b_score)
            else:
                print(f"Score out of bounds in prompt: {prompt} → A={a_score}, B={b_score}")
        else:
            print(f"Could not parse scores from entry with prompt: {prompt}")
    except Exception as e:
        print(f"Error parsing entry for prompt: {prompt}\n{e}")

# Show per-category averages
print("\nAverage Scores by Category:")
total_a = []
total_b = []

for cat, scores in category_scores.items():
    model_a_list = scores["model_a"]
    model_b_list = scores["model_b"]
    
    if model_a_list and model_b_list:
        avg_a = sum(model_a_list) / len(model_a_list)
        avg_b = sum(model_b_list) / len(model_b_list)
        print(f"\nCategory: {cat}")
        print(f"  Model A (gemma3:4b):  {avg_a:.2f} / 10 ({len(model_a_list)} samples)")
        print(f"  Model B (gemma3:27b): {avg_b:.2f} / 10 ({len(model_b_list)} samples)")

        total_a.extend(model_a_list)
        total_b.extend(model_b_list)

# Show overall averages
print("\nOverall Average Scores:")
overall_avg_a = sum(total_a) / len(total_a) if total_a else 0
overall_avg_b = sum(total_b) / len(total_b) if total_b else 0

print(f"  Model A (gemma3:4b):  {overall_avg_a:.2f} / 10 ({len(total_a)} samples)")
print(f"  Model B (gemma3:27b): {overall_avg_b:.2f} / 10 ({len(total_b)} samples)")
