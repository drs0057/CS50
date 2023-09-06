# TODO

from cs50 import get_string


def main():

    # Prompt user for text
    text = get_string("Text: ")

    # Gather letters, words, sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Use those to calculate index
    i = index(letters, words, sentences)

    # Print grade level
    if i >= 16:
        print("Grade 16+")

    elif i < 1:
        print("Before Grade 1")

    else:
        print(f"Grade {i}")


def index(letters, words, sentences):
    # Calulate average __ per 100 words
    aL = (letters / words) * 100
    aS = (sentences / words) * 100
    i = (0.0588 * aL) - (0.296 * aS) - 15.8

    return round(i)  # Round index to nearest int


def count_letters(text):

    # Count each char that is a letter
    count = 0
    for char in text:
        if char.isupper() or char.islower():
            count += 1
    return count


def count_words(text):

    # Count each word in the text
    count = 0
    for char in text:
        if char.isspace():
            count += 1
    count += 1  # The amount of spaces + 1 is the amount of words

    return count


def count_sentences(text):

    # Count number of sentences in text
    count = 0
    for char in text:
        if char == "!" or char == "?" or char == ".":
            count += 1

    return count


main()