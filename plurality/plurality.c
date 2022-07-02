#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote %i: ", i + 1);

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // If name matches one of the names of the candidates in the election, then update that candidate’s vote total to account for the new vote.
    for (int c = 0; c < candidate_count; c++)
    {
        if (strcmp(candidates[c].name, name) == 0)
        {
            candidates[c].votes++;
            // Return true to indicate a successful ballot.
            return true;
        }
    }

    // If name does not match the name of any of the candidates in the election, no vote totals should change.
    // Return false to indicate an invalid ballot.
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int highest_votes = 0;

    // Determine the highest votes nb among candidates
    for (int c = 0; c < candidate_count; c++)
    {
        if (highest_votes < candidates[c].votes)
        {
            highest_votes = candidates[c].votes;
        }
    }

    // Print out the name of all the candidates having the most votes (each one on e new line): if tie, several lines will be printed.
    for (int c = 0; c < candidate_count; c++)
    {
        if (highest_votes == candidates[c].votes)
        {
            printf("%s\n", candidates[c].name);
        }
    }
}