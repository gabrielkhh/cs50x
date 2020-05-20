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
bool checkedArr[MAX * (MAX - 1) / 2];
int pairsArrayOnSteroids[MAX * (MAX - 1) / 2][3];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void mergeSort(int left, int right, int arr[left][right]);
void merge(int l, int m, int r, int arr[l][r]);
void trace_path(int startingPoint, int nextPoint, int pathArr[pair_count], int index);

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
    //Loop through ranks[]
    for (int i = 0; i < candidate_count; i++)
    {
        //Fill in zero because it is not possible for a candidate to have more votes against himself/herself.
        //May be redundant code.
        preferences[i][i] = 0;
        for (int j = i + 1; j < candidate_count; j++)
        {
            //add 1 point to preferences comparing this ith candidate against i + 1th candidate (Other candidates that are less preferred against ith candidate) until the last choice of this one vote.
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    //integer for storing index counter for pairs array.
    int totalPairs = candidate_count * (candidate_count - 1) / 2;
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
                if (index_pairs_array < totalPairs)
                {
                    pairs[index_pairs_array].winner = i;
                    pairs[index_pairs_array].loser = j;
                    index_pairs_array++;
                    pair_count++;
                }

            }
            else if (scenario_two > scenario_one)
            {
                //jth candidate is more favourable over ith candidate.
                if (index_pairs_array < totalPairs)
                {
                    pairs[index_pairs_array].winner = j;
                    pairs[index_pairs_array].loser = i;
                    index_pairs_array++;
                    pair_count++;
                }
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int maxPairsCount = candidate_count * (candidate_count - 1) / 2;

    for (int i = 0; i < pair_count; i++)
    {
        int winID = pairs[i].winner;
        int loseID = pairs[i].loser;

        int wCount = preferences[winID][loseID];
        int lCount = preferences[loseID][winID];

        int result = wCount - lCount;

        pairsArrayOnSteroids[i][0] = result;
        pairsArrayOnSteroids[i][1] = winID;
        pairsArrayOnSteroids[i][2] = loseID;
    }
    mergeSort(0, pair_count, pairsArrayOnSteroids);

    for (int p = 0; p < pair_count; p++)
    {
        pairs[p].winner = pairsArrayOnSteroids[p][1];
        pairs[p].loser = pairsArrayOnSteroids[p][2];
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
    }

    int pathArr[pair_count];

    for (int a = 0; a < pair_count; a++)
    {
        checkedArr[a] = false;
    }

    for (int j = 0; j < pair_count; j++)
    {
        if (!checkedArr[j])
        {
            trace_path(pairs[j].winner, pairs[j].loser, pathArr, 0);
        }
    }
    return;
}

void trace_path(int startingPoint, int nextPoint, int pathArr[pair_count], int index)
{
    if (startingPoint == nextPoint)
    {
        //Path with highest index == lowest piority == remove the pair from completing the cycle.
        int highestIndexSoFar = 0;
        for (int m = 0; m < pair_count; m++)
        {
            if (pathArr[m] > highestIndexSoFar)
            {
                highestIndexSoFar = pathArr[m];
            }
        }
        locked[pairs[highestIndexSoFar].winner][pairs[highestIndexSoFar].loser] = false;
    }
    else
    {
        for (int n = 0; n < pair_count; n++)
        {
            if (pairs[n].winner == nextPoint)
            {
                checkedArr[n] = true;
                pathArr[index] = n;
                trace_path(startingPoint, pairs[n].loser, pathArr, index++);
            }
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    int boolCount = 0;
    if (candidate_count == 2)
    {
        int winnerID = pairs[0].winner;
        printf("%s\n", candidates[winnerID]);
    }
    else
    {
        //We know there are more than 2 candidates competing (0 or 1 candidate count is just dumb. lol)
        for (int i = 0; i < candidate_count; i++)
        {
            boolCount = 0;
            for (int j = 0; j < candidate_count; j++)
            {
                if (locked[j][i] == false)
                {
                    boolCount++;
                }
            }
            if (boolCount == candidate_count)
            {
                //Winner confirmed.
                printf("%s\n", candidates[i]);
            }
        }
    }
    return;
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int left, int right, int arr[right][3])
{
    if (left < right)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = left + (right - left) / 2;

        // Sort first and second halves
        mergeSort(left, m, arr);
        mergeSort(m + 1, right, arr);

        merge(left, m, right, arr);
    }
}

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int l, int m, int r, int arr[r][3])
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int L[n1][3], R[n2][3];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
    {
        L[i][0] = arr[l + i][0];
        L[i][1] = arr[l + i][1];
        L[i][2] = arr[l + i][2];
    }

    for (j = 0; j < n2; j++)
    {
        R[j][0] = arr[m + 1 + j][0];
        R[j][1] = arr[m + 1 + j][1];
        R[j][2] = arr[m + 1 + j][2];
    }

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i][0] >= R[j][0])
        {
            arr[k][0] = L[i][0];
            arr[k][1] = L[i][1];
            arr[k][2] = L[i][2];
            i++;
        }
        else
        {
            arr[k][0] = R[j][0];
            arr[k][1] = R[j][1];
            arr[k][2] = R[j][2];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k][0] = L[i][0];
        arr[k][1] = L[i][1];
        arr[k][2] = L[i][2];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k][0] = R[j][0];
        arr[k][1] = R[j][1];
        arr[k][2] = R[j][2];
        j++;
        k++;
    }
}

