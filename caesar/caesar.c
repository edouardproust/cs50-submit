#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print_cypher(string prefix, string str, string key);
char encrypt_char(char c, int key);
bool is_int(string str);

int main(int argc, string argv[])
{
    int args_nb = argc - 1; // nb of arguments entered by the user
    string key = argv[1]; // the key entered as an argument by the user

    // Check that only one argument is defined and is an int
    if (args_nb != 1 || !is_int(key))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // Prompt user for the text to encrypt
    string plain;
    do
    {
        plain = get_string("plaintext: ");
    }
    while (strlen(plain) < 1);

    // encrypt plain text and print the result
    print_cypher("ciphertext: ", plain, key); // convert key to int (uses `stdlib.h` lib) + remove overflow (using modulo)
    return 0;
}


void print_cypher(string prefix, string str, string key)
{
    // convert key to an int and remove overflow (if `(int) key > (int) 'z'`)
    int k = atoi(key) % 26; // 26 == 'z' - 'a' + 1 == letters in the alphabet

    // Encrypt and print the result
    printf("%s", prefix);
    for (int i = 0; i < strlen(str); i++)
    {
        printf("%c", encrypt_char(str[i], k));
    }
    printf("\n");
}

/**
 * Encrypt 1 character
 */
char encrypt_char(char c, int key)
{
    int result;
    int z = isupper(c) ? 'Z' : 'z';
    int a = isupper(c) ? 'A' : 'a';

    // Don't modify non-alpha characters
    if (!isalpha(c))
    {
        return c;
    }
    // If the new char is higher than 'z' (overflow), then get the overflow and add it to 'a'
    int overflow = c + key - z;
    return overflow > 0 ? a + overflow - 1 : c + key;
}

/**
 * Check if a string is an integer (contains only digits)
 */
bool is_int(string str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}