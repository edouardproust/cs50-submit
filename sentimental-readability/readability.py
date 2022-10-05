from cs50 import get_string


def main():
    # Get input
    text = get_string("Text: ")
    # Calculate letters, words and sentences number
    letters_nb = get_letters_nb(text)
    words_nb = get_words_nb(text)
    sentences_nb = get_sentences_nb(text)
    # Calculate and print the grade
    grade = get_grade(letters_nb, words_nb, sentences_nb)
    print_grade(grade)


def get_grade(letters_nb, words_nb, sentences_nb):
    """Calculate and returns the text readability grade based on the Coleman-Liau index"""
    L = letters_nb / words_nb * 100  # Average number of letters per 100 words in the text
    S = sentences_nb / words_nb * 100  # average number of sentences per 100 words in the text
    return 0.0588 * L - 0.296 * S - 15.8


def print_grade(grade):
    if grade < 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade {round(grade)}")


def get_letters_nb(text):
    nb = 0
    # Each letter must be an alpha character to be considered as such
    for char in text:
        if char.isalpha():
            nb += 1
    return nb


def get_words_nb(text):
    nb = 0
    words = text.split()
    # Each word must contains at least 1 alpha char to be considered as such
    for word in words:
        has_alpha_chars = False
        for char in word:
            if char.isalpha():
                has_alpha_chars = True
        if has_alpha_chars:
            nb += 1
    return nb


def get_sentences_nb(text):
    nb = 0
    for char in text:
        if char in [".", "!", "?"]:
            nb += 1
    return nb


if __name__ == "__main__":
    main()
