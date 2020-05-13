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
        string name = get_string("Vote: ");

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
    for (int i = 0; i < MAX; i++)
    {
        if (candidates[i].name != 0)
        {
            if (strcmp(candidates[i].name, name) == 0)
            {
                candidates[i].votes += 1;
                return true;
            }
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    char *winners[MAX];
    int arrayCounter = 0;
    int HighestCount = 0;
    for (int i = 0; i < MAX; i++)
    {
        if (candidates[i].votes > HighestCount)
        {
            HighestCount = candidates[i].votes;
            //clear array of potential winners
            for (int j = 0; j < MAX; j++)
            {
                winners[j] = 0;
            }
            arrayCounter = 0;
            //Add the new name to winners
            winners[arrayCounter] = candidates[i].name;
            arrayCounter++;
        }
        else if (candidates[i].votes == HighestCount)
        {
            //We have at least 2 or more highest vote counts so far
            winners[arrayCounter] = candidates[i].name;
            arrayCounter++;
        }
    }

    if (HighestCount == 0)
    {
        printf("Invalid election. No votes were given.\n");
        return;
    }

    for (int k = 0; k < MAX; k++)
    {
        if (winners[k] == 0)
        {
            break;
        }
        else
        {
            printf("%s\n", winners[k]);
        }
    }
    return;
}

