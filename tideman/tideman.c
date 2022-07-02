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

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
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
bool circle_occures(int cycle_start, int loser);
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
    for (int i = 0; i < candidate_count; i++)
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
                // Create a pair && set winner and loser
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }

    if (DEBUG)
    {
        debug_pairs("(unsorted) ");
    }

    return;
}

/**
 * Sort pairs in decreasing order by strength of victory
 */
void sort_pairs(void)
{
    // For each pair (`c` stands for current). Start at 1 as we then compare with the previous pair: p = c - 1 (`p` stands for previous)
    for (int c = 1; c < pair_count; c++)
    {
        // Save current pair before further updates of its value
        pair saved_cur_pair = pairs[c];

        // For each previous pair (decreasing loop)
        // Compare the amount of people who prefer winner to loser in the current pair with the previous pair's one
        int p;
        for (p = c - 1; p >= 0; p--)
        {
            // vars
            int l_prev = pairs[p].loser;
            int l_cur = pairs[c].loser;
            int w_prev = pairs[p].winner;
            int w_cur = pairs[c].winner;

            // If more people prefer winner to loser in the current pair than in the previous one: replace current pair by the previous one.
            if (preferences[w_cur][l_cur] > preferences[w_prev][l_prev])
            {
                pairs[c] = pairs[p];
            }
            else
            {
                break; // Stop comparision
            }
        }
        pairs[p + 1] = saved_cur_pair;
    }

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
    // For each pair
    for (int i = 0; i < pair_count; i++)
    {
        // Lock the pair only if no circle occures
        if (!circle_occures(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

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
    for (int i = 0; i < candidate_count; i++)
    {
        bool opposite_pairs = false;

        // Print the first candidate whose all opposite pairs are false
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true) // locked[j][i] is the opposite pair (of locked[i][j])
            {
                opposite_pairs = true;
                break;
            }
        }
        if (opposite_pairs == false)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}

/**
 * Check if the given pairs make a circle
 */
bool circle_occures(int first, int loser)
{
    // Check the first entry
    if (loser == first)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (circle_occures(first, i))
            {
                return true;
            }
        }
    }
    return false;
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
    printf("}\n");
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
        printf("   %i: { 'winner': %i, 'loser': %i }\n", i, pairs[i].winner, pairs[i].loser);
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
    printf("}\n\n");
    return;
}