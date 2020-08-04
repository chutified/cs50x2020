#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

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
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool validNotCycle(int winner, int loser);

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

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

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

        record_preferences(ranks);
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // find  the candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            // candidate found
            ranks[rank] = i;
            return true;
        }
    }

    // candidate not found
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // range over ranks
    for (int i = 0; i < candidate_count; i++)
    {
        // getting not prefered candidatees
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // range over all pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {

            // get preferences
            int ij = preferences[i][j];
            int ji = preferences[j][i];

            // construct the pair
            pair p;
            if (ij > ji)
            {
                p.winner = i;
                p.loser = j;

                // append result
                pairs[pair_count] = p;
                pair_count++;
            }
            else if (ij < ji)
            {
                p.winner = j;
                p.loser = i;

                // append result
                pairs[pair_count] = p;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // set default values
        int bestVal = preferences[pairs[i].winner][pairs[i].loser];
        int bestPos = i;

        // select highest
        for (int j = i + 1; j < pair_count; j++)
        {
            int p = preferences[pairs[j].winner][pairs[j].loser];
            if (p > bestVal)
            {
                bestVal = p;
                bestPos = j;
            }
        }

        // swap with the ith
        pair temp = pairs[i];
        pairs[i] = pairs[bestPos];
        pairs[bestPos] = temp;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // validate
        if (validNotCycle(pairs[i].winner, pairs[i].loser) == true)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// validNotCycle returns true if locker[winner][loser] = true does not create a cycle.
bool validNotCycle(int winner, int loser)
{
    // base
    if (locked[loser][winner] == true)
    {
        return false;
    }

    for (int i = 0; i < candidate_count; i ++)
    {
        if (locked[i][winner])
        {
            return validNotCycle(i, loser);
        }
    }

    // not source
    return true;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool winner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                winner = false;
            }
        }

        if (winner)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}