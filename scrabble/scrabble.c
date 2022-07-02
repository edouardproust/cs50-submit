#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);
int char_score(char c);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    int score = 0;
    // Loop through each char of the word
    for (int i = 0; i < strlen(word); i++)
    {
        // Add the char's points to the score
        score += char_score(word[i]);
    }
    return score;
}

int char_score(char c)
{
    int alph_char = 'a', alph_index = 0;

    // Loop through the letters of the alphabet until the char matches one
    while (
        alph_char <= 'z'
        && c != alph_char // check lowercase letters
        && c != alph_char + ('A' - 'a') // check uppercase letters too
    )
    {
        alph_char++; // move to the next letter of the alphabet
        alph_index++;
    }
    return POINTS[alph_index];
}