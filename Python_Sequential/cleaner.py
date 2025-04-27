#!/usr/bin/env python3
# arguments: inputFileName outputFileName
# it works just for English files

import sys 
from time import time

# the list of stop words was provided by Deepseek
STOP_WORDS = {
    "a", "about", "above", "after", "again", "against", "all", "am", "an", "and",
    "any", "are", "aren't", "as", "at", "be", "because", "been", "before", "being",
    "below", "between", "both", "but", "by", "can", "can't", "cannot", "could",
    "couldn't", "did", "didn't", "do", "does", "doesn't", "doing", "don't", "down",
    "during", "each", "few", "for", "from", "further", "had", "hadn't", "has",
    "hasn't", "have", "haven't", "having", "he", "he'd", "he'll", "he's", "her",
    "here", "here's", "hers", "herself", "him", "himself", "his", "how", "how's",
    "i", "i'd", "i'll", "i'm", "i've", "if", "in", "into", "is", "isn't", "it",
    "it's", "its", "itself", "let's", "me", "more", "most", "mustn't", "my",
    "myself", "no", "nor", "not", "of", "off", "on", "once", "only", "or", "other",
    "ought", "our", "ours", "ourselves", "out", "over", "own", "same", "shan't",
    "she", "she'd", "she'll", "she's", "should", "shouldn't", "so", "some", "such",
    "than", "that", "that's", "the", "their", "theirs", "them", "themselves", "then",
    "there", "there's", "these", "they", "they'd", "they'll", "they're", "they've",
    "this", "those", "through", "to", "too", "under", "until", "up", "very", "was",
    "wasn't", "we", "we'd", "we'll", "we're", "we've", "were", "weren't", "what",
    "what's", "when", "when's", "where", "where's", "which", "while", "who", "who's",
    "whom", "why", "why's", "with", "won't", "would", "wouldn't", "you", "you'd",
    "you'll", "you're", "you've", "your", "yours", "yourself", "yourselves"
}

def remove_punctuation(S:str):
    punctuation = {"$", ",", ".", ";",
                   ":", "+", "-", "*", "/",
                   "=", "¿", "?", "¡", "!",
                   "(", ")", "[", "]", "\\", 
                   "\"", "°", "#", "&", "%"}
    result = ""
    for s in S:
        # Using hashing with sets is faster. In average O(1)
        if not s in punctuation: 
            result += s.lower() # putting everything in lower case
    return result

def remove_stop_words(L:list):
    if not len(L): return "\n"
    result = []
    for word in L:
        word = remove_punctuation(word)
        # Using hashing with sets is faster. In average O(1)
        if len(word) and not word in STOP_WORDS: result.append(word)
    return " ".join(result) + "\n"


if __name__ == "__main__":
    # inputFile = input("Write the name of your input file:\n").strip()
    # outputFile = input("Write the name of your output file:\n").strip()
    inputFile = sys.argv[1]
    outputFile = sys.argv[2]

    start_time = time()

    with open(inputFile, 'r', encoding="utf8") as f:
        lines = f.readlines()

    # Tokenization
    lines = [line.strip().split() for line in lines]

    # Remove stop words
    with open(outputFile, "w", encoding="utf8") as f:
        for line in lines:
            f.write(remove_stop_words(line))

    end_time = time()
    print(f"Ready!\n{len(lines)} lines processed\n{end_time - start_time} seconds")