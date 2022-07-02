#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9
#define DEBUG true

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

int results[MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
    int strength;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
void clear_locked(void);
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void debug_ranks(int ranks[]);
void debug_preferences(void);
void debug_pairs(string prefix);
void debug_locked(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    clear_locked();

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        // Debug
        if (DEBUG)
        {
            debug_ranks(ranks);
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

/**
 * Clear graph of locked in pairs
 */
void clear_locked(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }
    return;
}

/**
 * Update ranks given a new vote
 */
bool vote(int rank, string name, int ranks[])
{
    // Check if the name exist (valid ballot). If not, return false
    for (int i = 0;i < candidate_count; i++)
    {
        // Check if the candidate's name exists in `candidates[]`
        if (strcmp(name, candidates[i]) == 0)
        {
            // Check that each name is entered once
            for (int r = 0; r < rank; r++)
            {
                if (ranks[r] == i)
                {
                    return false;
                }
            }
            // Else: save the rank
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

/**
 * Update preferences given one voter's ranks
 * @param ranks[] eg. {0, 2, 1}
 */
void record_preferences(int ranks[])
{
    // For each rank
    for (int i = 0; i < candidate_count; i++)
    {
        int cand_main = ranks[i]; // get index for the candidate to be compared (called "main")
        // For each candidate
        for (int j = 0; j < candidate_count; j++)
        {
            int cand_comp = ranks[j]; // get index for the candidate to compare (called "comp")
            // If is not the same candidate and is below of main candidate
            if (cand_main != cand_comp && j > i)
            {
                // Add 1 preference for these candidates' combination
                preferences[cand_main][cand_comp] += 1;
            }
        }
    }
    // Debug
    if (DEBUG)
    {
        debug_preferences();
    }

    return;
}

/**
 * Record pairs of candidates where one is preferred over the other
 */
void add_pairs(void)
{

    // For each preference
    for (int i = 0; i < candidate_count; i++)
    {
        // For each index
        for (int j = 0; j < candidate_count; j++)
        {
            // Check that is not Tie (score is bigger than the one of its opposite pair)
            if (preferences[i][j] > preferences[j][i])
            {
                // Create a pair && set winner, loser and strength of victory
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pairs[pair_count].strength = preferences[i][j] - preferences[j][i];
                pair_count++;
            }
        }
    }
    // Debug
    if (DEBUG)
    {
        debug_pairs("(unsorted) ");
    }

    return;
}

/**
 * Sort pairs in decreasing order by strength of victory
 * Used algorythm: selection sort
 */
void sort_pairs(void)
{
    int sorted_pairs[pair_count];
    // For each pair
    for (int i = 0; i < pair_count; i++)
    {
        // Define highest strength
        for (int j = i+1; j < pair_count; j++)
        {
            // if strength is higher than the ones of the other pairs
            if (pairs[j].strength > pairs[i].strength)
            {
                pair toMoveDown = pairs[j];
                pair toMoveUp = pairs[i];
                pairs[i] = toMoveDown;
                pairs[j] = toMoveUp;
            }
        }
    }
    // Debug
    if (DEBUG)
    {
        debug_pairs("(sorted) ");
    }

    return;
}

/**
 * Lock pairs into the candidate graph in order, without creating cycles
 */
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        results[i] += 1;
    }
    // Debug
    if (DEBUG)
    {
        debug_locked();
    }
    return;
}

/**
 * Print the winner of the election
 */
void print_winner(void)
{
    // Print the winner of the locked pair that has the most `true` values
    printf("Results: ");
    for (int i = 0; i < pair_count; i++)
    {
        printf("%i %i / ", i, results[i]);
    }
    printf("\n");

    // If TIE: print the first one

    return;
}

// DEBUG FUNCTIONS ========================================================

void debug_ranks(int ranks[])
{
    printf("\nranks: { ");
    for (int i = 0; i < candidate_count; i++)
    {
        printf("%i", ranks[i]);
        if (i < candidate_count - 1)
        {
            printf(", ");
        }
    }
    printf(" }\n");
    return;
}

void debug_preferences(void)
{
    printf("preferences: { \n");
    for (int i = 0; i < candidate_count; i++)
    {
        printf("   %i: { ", i);
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i", preferences[i][j]);
            if (j < candidate_count - 1)
            {
                printf(", ");
            }
        }
        printf(" }\n");
    }
    printf("}");
    return;
}

/**
 * @param prefix string Print text at the begining of the debug text
 */
void debug_pairs(string prefix)
{
    printf("%spairs: { \n", prefix);
    for (int i = 0; i < pair_count; i++)
    {
        printf("   %i: { 'winner': %i, 'loser': %i, 'strength': %i }\n", i, pairs[i].winner, pairs[i].loser, pairs[i].strength);
    }
    printf("}\n");
    return;
}


void debug_locked(void)
{
    printf("locked: { \n");
    for (int i = 0; i < pair_count; i++)
    {
        printf("   %i: { ", i);
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%i", locked[i][j]);
            if (j < candidate_count - 1)
            {
                printf(", ");
            }
        }
        printf(" }\n");
    }
    printf("}\n");
    return;
}
