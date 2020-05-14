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

        printf("\n");
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
    // TODO
    //Loop through the candidates array and check if the given name matches any of the names in the array of candidates.
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    //Loop through ranks[]
    for (int i = 0; i < candidate_count; i++)
    {
        //Fill in zero because it is not possible for a candidate to have more votes against himself/herself.
        preferences[i][i] = 0;
        //if (i < candidate_count - 2)
        //{
            for (int j = i + 1; j < candidate_count; j++)
            {
                //add 1 point to preferences comparing this ith candidate against i + 1th candidate (Other candidates that are less preferred against ith candidate) until the last choice of this one vote.
                preferences[ranks[i]][ranks[j]] += 1;
            }
        //}
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    //integer for storing index counter for pairs array.
    int totalCount = candidate_count * (candidate_count - 1) / 2;
    int index_pairs_array = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            //Get the number of preferences for each candidate over the other.
            int scenario_one = preferences[i][j];
            int scenario_two = preferences[j][i];

            //Compare the results to determine which candidate is more favourable.
            if (scenario_one > scenario_two)
            {
                //ith candidate is more favourable over jth candidate.
                if (index_pairs_array < totalCount)
                {
                    pairs[index_pairs_array].winner = i;
                    pairs[index_pairs_array].loser = j;
                }

            }
            else if (scenario_two > scenario_one)
            {
                //jth candidate is more favourable over ith candidate.
                if (index_pairs_array < totalCount)
                {
                    pairs[index_pairs_array].winner = j;
                    pairs[index_pairs_array].loser = i;
                }
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

int next_rank(int index)
{
    if (index < candidate_count)
    {
        return next_rank(index + 1);
    }
    else
    {
        return 10;
    }
}

