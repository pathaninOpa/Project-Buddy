import numpy as np

def levenshtein_distance(ref, hyp):
    """
    Calculates the Levenshtein distance between two sequences.
    """
    m, n = len(ref), len(hyp)
    d = np.zeros((m + 1, n + 1), dtype=int)

    for i in range(m + 1):
        d[i, 0] = i
    for j in range(n + 1):
        d[0, j] = j

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if ref[i - 1] == hyp[j - 1]:
                cost = 0
            else:
                cost = 1
            d[i, j] = min(d[i - 1, j] + 1,      # deletion
                          d[i, j - 1] + 1,      # insertion
                          d[i - 1, j - 1] + cost) # substitution
    return d[m, n]

def calculate_wer(reference, hypothesis):
    """
    Calculates Word Error Rate (WER).
    """
    ref_words = reference.split()
    hyp_words = hypothesis.split()
    
    if not ref_words:
        return 1.0 if hyp_words else 0.0
        
    dist = levenshtein_distance(ref_words, hyp_words)
    return dist / len(ref_words)

def calculate_cer(reference, hypothesis):
    """
    Calculates Character Error Rate (CER).
    """
    # For Thai, we often compare character by character directly, 
    # ignoring spaces if the reference doesn't use them consistently,
    # but strictly speaking, CER includes all characters.
    # We will strip whitespace for a cleaner character comparison.
    ref_chars = list(reference.replace(" ", ""))
    hyp_chars = list(hypothesis.replace(" ", ""))
    
    if not ref_chars:
        return 1.0 if hyp_chars else 0.0

    dist = levenshtein_distance(ref_chars, hyp_chars)
    return dist / len(ref_chars)
