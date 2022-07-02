#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Get the text (input)
    string text;
    do
    {
        text = get_string("Text: ");
    }
    while (strlen(text) < 1); // check that a text as been entered (at least 1 char)

    // Apply the Coleman-Liau index: `index = 0.0588 * L - 0.296 * S - 15.8`
    // where L is the average number of letters per 100 words in the text, and S is the average number of sentences per 100 words in the text.
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    float letters_avg = (float) letters / (float) words * 100;
    float sentences_avg = (float) sentences / (float) words * 100.0;
    int grade = round(0.0588 * letters_avg - 0.296 * sentences_avg - 15.8);

    // Set the limits and print the result
    if (grade > 16)
    {
        printf("Grade 16+");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1");
    }
    else
    {
        printf("Grade %i", grade);
    }
    printf("\n");

    // Debug:
    // printf("w: %i\n l: %i\ns: %i\nL: %f\nS: %f\n", words, letters, sentences, letters_avg, sentences_avg);
}

int count_letters(string text)
{
    int i = 0; // char index
    int letters_count = 0; // init. number of letters

    // move through each char until the end of the string
    while (text[i] != '\0')
    {
        // Add 1 to the letters_count if the char is alphanumeric
        if (isalpha(text[i]))
        {
            letters_count++; // add a letter to the count
        }
        i++; // move to next char
    }
    return letters_count;
}

int count_words(string text)
{
    int i = 0;
    int words_count = 0;
    int current_word_length = 0; // To count a word only if there are letters between 2 spaces

    while (text[i] != '\0')
    {
        if (text[i] == ' ' && current_word_length > 0)
        {
            words_count++;
            current_word_length = 0;
        }
        else if (isalpha(text[i]))
        {
            current_word_length++;
        }
        i++;
    }
    if (current_word_length > 0) // Add the last word
    {
        words_count++;
    }
    return words_count;
}

int count_sentences(string text)
{
    int i = 0;
    int sentences_count = 0;
    int current_sentence_length = 0; // To count a word only if there are letters between 2 spaces

    while (text[i] != '\0')
    {
        if ((text[i] == '.' || text[i] == '!' || text[i] == '?') && current_sentence_length > 0)
        {
            sentences_count++;
            current_sentence_length = 0;
        }
        else if (isalpha(text[i]))
        {
            current_sentence_length++;
        }
        i++;
    }
    if (current_sentence_length > 0) // Add the last sentence if it the ponctuation is missing
    {
        sentences_count++;
    }
    return sentences_count;
}