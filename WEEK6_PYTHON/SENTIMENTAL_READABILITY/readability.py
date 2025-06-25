from cs50 import get_string
import sys


def count_letters_words_sentences(text):
    letters = 0
    sentences = 0

    for c in text:
        if c.isalpha():
            letters += 1
        elif c in ['.', '!', '?']:
            sentences += 1

    words = len(text.split())

    return letters, words, sentences


# PROMPT THE USER FOR SOME TEXT
prompt = get_string("Text: ")

# COUNT THE NUMBER OF LETTERS, WORDS, AND SENTENCES IN THE TEXT
letters, words, sentences = count_letters_words_sentences(prompt)

# IF THERE ARE NO WORDS
if words == 0:
    sys.exit("No words to evaluate!")

# COMPUTE THE COLEMAN-LIAU INDEX
L = letters / words * 100
S = sentences / words * 100
index = 0.0588 * L - 0.296 * S - 15.8

# PRINT THE GRADE LEVEL
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {round(index)}")
