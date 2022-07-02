#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void print_cipher(string prefix, string str, string key);
char encrypt_char(char c, string key);
bool is_valid_key(string key);

int main(int argc, string argv[])
{
    int args_nb = argc - 1; // nb of arguments entered by the user
    string key = argv[1]; // the key entered as an argument by the user

    // Check that only one argument has been entered by the user
    if (args_nb != 1)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // Check that the entered key is valid
    else if (!is_valid_key(key))
    {
        return 1;
    }

    // Prompt user for the text to encrypt
    string plaintext;
    do
    {
        plaintext = get_string("plaintext: ");
    }
    while (strlen(plaintext) < 1);

    // encrypt plaintext and print the result
    print_cipher("ciphertext: ", plaintext, key);
    return 0;
}


void print_cipher(string prefix, string str, string key)
{
    // Encrypt and print the result
    printf("%s", prefix);
    for (int i = 0; i < strlen(str); i++)
    {
        printf("%c", encrypt_char(str[i], key));
    }
    printf("\n");
}

/**
 * Encrypt 1 character
 */
char encrypt_char(char c, string key)
{
    // return the char if is not a letter
    if (!isalpha(c))
    {
        return c;
    }
    // get index of this char in the alphabet
    int alph_char = 'a', alph_index = 0;
    while (
        alph_char <= 'z'
        && c != alph_char
        && c != alph_char + ('A' - 'a')
    )
    {
        alph_char++;
        alph_index++;
    }
    // get the char for this index inside the cipher key
    char cipher_char = key[alph_index];

    // Return the result with the appropriate case
    return islower(c) ? tolower(cipher_char) : toupper(cipher_char);
}

/**
 * Check that the key typed by the user is valid
 */
bool is_valid_key(string key)
{
    int i = 0;
    int length = strlen(key);

    // Check the key contains 26 char.
    if (length != 26)
    {
        printf("Key must contain 26 characters.\n");
        return false;
    }

    // check that each char is a letter
    while (key[i] != '\0')
    {
        if (!isalpha(key[i]))
        {
            printf("Key must contain letters only.\n");
            return false;
        }
        i++;
    }

    // Check that each letter appears exactly once
    for (char c = 'a'; c <= 'z'; c++)
    {
        int j = 0, times = 0;
        while (key[j] != '\0')
        {
            if (key[j] == c || key[j] == c + ('A' - 'a'))
            {
                times++;
            }
            j++;
        }
        if (times != 1)
        {
            printf("The key must contain each letter exactly once.\n");
            return false;
        }
    }

    return true;
}