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
bool cycle_check(int x, int y);

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

    //initialize all preferences values to be zero
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            preferences[i][j] = 0;
        }
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
    int x = 0;
    for (int n = 0; n < candidate_count; n++)
    {
        if (strcmp(candidates[n], name) == 0)
        {
            ranks[rank] = n; //ranks[0] will be the voter's 1st preference; n is the index of their 1st preference
            x = 1; //keeps record of whether the name was valid
        }
    }

    // TODO
    if (x == 1)
    {
        return true;
    }
    else
    {
        return false;
    }

}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{

    //input voter's preferences
    for (int n = 0; n < candidate_count; n++) //will compare all pairs of the voter's preferences
    {
        for (int k = 0; k < candidate_count - (n + 1); k++) //ranks[n] is preferred over ranks[n + k + 1] for this loop
        {
            preferences[ranks[n]][ranks[n + k + 1]]++;
        }
    }
    // TODO
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int n = 0; n < candidate_count; n++)
    {
        for (int k = 0; k < n; k++)
        {
            if (preferences[n][k] > preferences[k][n])
            {
                pairs[pair_count].winner = n;
                pairs[pair_count].loser = k;
                pair_count++;
            }
            else if (preferences[n][k] < preferences[k][n])
            {
                pairs[pair_count].winner = k;
                pairs[pair_count].loser = n;
                pair_count++;
            }

        }
    }

    // TODO
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int mag1 = 0; //magnitude of the win in first pair
    int mag2 = 0; //magnitude of the win in next pair
    int i1 = 0; //intermeditate int used to switch values
    int i2 = 0; //intermeditate int used to switch values
    //Bubble sort for simplicity
    for (int x = 0; x < pair_count - 1; x++)
    {
        for(int y = 0; y < pair_count - 1; y++)
        {
            mag1 = (preferences[pairs[y].winner][pairs[y].loser]) - (preferences[pairs[y].loser][pairs[y].winner]);
            mag2 = (preferences[pairs[y + 1].winner][pairs[y + 1].loser]) - (preferences[pairs[y + 1].loser][pairs[y + 1].winner]);
            if (mag2 > mag1) //if true, then switch
            {
                //placeholders
                i1 = pairs[y].winner;
                i2 = pairs[y].loser;
                //swap values
                pairs[y].winner = pairs[y + 1].winner;
                pairs[y].loser = pairs[y + 1].loser;
                pairs[y + 1].winner = i1;
                pairs[y + 1].loser = i2;
            }
            // if mag2 = mag1 or mag2 < mag1, do nothing
        }
    }
    // TODO
    return;
}

//function to check if a cycle was created upon last entry, takes winner index and loser index for a given pair as its arguements
bool cycle_check(int start, int end)
{
    if (start == end) // base case
    {
        return true;
    }

    for (int n = 0; n < candidate_count; n++) //find where the loser won
    {
        if(locked[end][n])
        {
            if (cycle_check(start, n))
            {
                return true;
            }
        }
    }

    return false;

}
// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{

    for (int k = 0; k < pair_count; k++) //locks in winners
    {
        locked[pairs[k].winner][pairs[k].loser] = true;

        if (cycle_check(pairs[k].winner, pairs[k].loser)) //if a cycle now exists, negate what was just done
        {
            locked[pairs[k].winner][pairs[k].loser] = false;
        }
    }

    // TODO
    return;
}


// Print the winner of the election
void print_winner(void)
{
int tc = 0;
    for (int j = 0; j < candidate_count; j++) //finds the row with no "trues". This is the winner
    {
       for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][j] == true)
            {
                tc++;
            }

        }

        if (tc == 0)
        {
            printf("%s\n", candidates[j]);
        }
        tc = 0; //reset true count
    }

    // TODO
    return;
}