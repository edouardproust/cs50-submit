// Implements a dictionary's functionality
// For a quick test, use command: ./speller dictionaries/small texts/a.txt

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
// I chose 5381. Explanations:
// - Based on my own manual tests,, i found that 5000 was the quickest number or running the hash function below.
// - Later on i found the djd hash function which is using the constant 5381 which is a number close to 5000.
// - Using 5381 made the algorythm sometimes a bit quicker than using 5000, with a 1 sec. gain. This needs to be proven matehmaticaly though (i didn't go that further)
const unsigned int N = 5381;

// Hash table
node *table[N];

// Number of words
int words = 0;

// Hashes word to a number
// - Average time for the lalaland.txt file: 0.11 (staff's solution: 0.07)
// - Based Dan Bernstein’s djb2 algorithm: http://www.cse.yorku.ca/~oz/hash.html (Explanations: https://theartincode.stanis.me/008-djb2/)
unsigned int hash(const char *word)
{
    // Creating a seed variable (for the loop below and speed purpose)
    int hash = N;

    // Loop over every character of the word
    int c;y
    
    while ((c = *word++))
    {
        // Hash each character of the word (and pile up the result inside the `hash` variable)
        // - `hash << x` is a "bit shift" (it is like doing `hash * 2^x` but it's faster)
        // - I didn't see any speed improvements changing the value of `x` so i kept the smallest one which is 1
        hash = (hash << 1) + toupper(c); // Case insensitive: using toupper()
    }

    // Ensure that the final `hash` result is contained inside the hash table boundaries (N)
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
// = Add each word of the dictionnary to the hash table
bool load(const char *dictionary)
{

    FILE *file = fopen(dictionary, "r"); // Get the pointer to the dictionnary
    if (file == NULL) // Check if file exists
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    char word[LENGTH + 1], c;
    int index = 0;
    while (fread(&c, sizeof(char), 1, file)) // Loop over each character of the dictionary
    {
        // Build each word
        if (c != '\n')
        {
            word[index] = c; // Append character to word
            index++; // Loop to the next character
            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                while (fread(&c, sizeof(char), 1, file) && isalpha(c)); // Consume remainder of alphabetical string
                index = 0; // Prepare for new word
            }
        }

        // Add each new word to the hash table
        else if (index > 0) // We must have found a whole word
        {
            // Finish building the word
            word[index] = '\0'; // Terminate current word
            index = 0; // Prepare for next word
            words++; // Increase the number of words by 1;

            // Add the word to the hash table
            int wIndex = hash(word); // get the index of the list the word will be added to
            node *newNode = malloc(sizeof(node)); // Create a new node
            if (newNode == NULL)
            {
                return false;
            }
            strcpy(newNode->word, word); // We can't assign to an array, only copy to it, so we need to use strcpy() func.
            newNode->next = table[wIndex]; // point to the current head of the list
            table[wIndex] = newNode; // Add The node at the begenning of the list (update list's head)
        }
    }

    // Check for errors during the process
    if (ferror(file))
    {
        fclose(file);
        printf("Error reading %s.\n", dictionary);
        return false;
    }
    // Success
    fclose(file);
    return true;
}


// Returns true if word is in dictionary, else false
bool check(const char *word)
{

    int wIndex = hash(word);
    node *cursor = table[wIndex];

    while (cursor != NULL)
    {
        //check if the word matches
        if (strcasecmp(word, cursor->word) == 0) // case insensitive (using strcascpm)
        {
            return true;
        }
        //move cursor to next node
        cursor = cursor->next;
    }
    return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
// The number of words is counted in the load() function. The size() just allows to get the result.
unsigned int size(void)
{
    return words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        // Free all nodes recursively (until we reach the end of the list)
        while (cursor != NULL)
        {
            node *tmp = cursor; // Create temp
            cursor = cursor->next; // Move cursor to next node
            free(tmp); // Free up temp
        }
        // If we reached the end of the list
        if (i == N - 1 && cursor == NULL)
        {
            return true;
        }
    }
    return false;
}