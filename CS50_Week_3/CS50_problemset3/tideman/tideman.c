// Holds a Tideman election.

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
} pair;

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
bool helper(int current, int check, bool tracker[]);

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
    // Iterate through each candidate.
    for (int i = 0; i < candidate_count; i++)
    {
        // Check to see if the name the voter entered matches a candidate. If so note which rank this voter gives him/her and return true.
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[i] = rank;
            return true;
        }
    }
    // Name dosen't match any candidate in the election, return false.
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Iterate through each candidate.
    for (int i = 0; i < candidate_count; i++)
    {
        // Iterate through the candidates again so that we can compare candidate's i rank to every other candidates rank.
        for (int j = 0; j < candidate_count; j++)
        {
            // If candidate's i rank is less than candidates[j]'s rank add one to preferences to keep track of how many people prefer this candidate (candidate[i]) over candidate[j].
            if (ranks[i] < ranks[j])
            {
                preferences[i][j]++;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Iterate through each candidate.
    for (int i = 0; i < candidate_count; i++)
    {
        // Iterate through each candidate again. (Allows us to compare each candidate to every other candidate.)
        for (int j = 0; j < candidate_count; j++)
        {
            // If candidate i is more preffered than candidate j, note this in the pairs array and increment pair_count.
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair temp;
    // Iterate through pair count to keep track of which pair we've currently sorted up to.
    for (int i = 0; i < pair_count - 1; i++)
    {
        // Highest index we've currently sorted up to.
        int max_index = i;
        // Iterate again to compare the next pair of candidates in the array to pair i.
        for (int j = i + 1; j < pair_count; j++)
        {
            int current_diff = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            int strongest_diff = preferences[pairs[max_index].winner][pairs[max_index].loser] - preferences[pairs[max_index].loser][pairs[max_index].winner];
            if (current_diff > strongest_diff)
            {
                max_index = j;
            }
        }
        // Swap pairs[i] with pairs[max_index]
        temp = pairs[i];
        pairs[i] = pairs[max_index];
        pairs[max_index] = temp;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Tracker tracks whether we've checked a pair for a cycle within helper yet. Prevents infinite recursive loop.
    bool track[candidate_count];

    for (int i = 0; i < candidate_count; i++)
    {
        track[i] = false;
    }

    // Iterate through each pair and if locking that pair in dosen't result in a cycle, then lock it.
    for (int i = 0; i < pair_count; i++)
    {
        if (helper(pairs[i].winner, pairs[i].loser, track))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
        else
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    bool detect_winner[candidate_count];

    for (int i = 0; i < candidate_count; i++)
    {
        detect_winner[i] = false;
    }

    // Go through each pair of candidates and keep track of how many times a particular candidate is locked in over another one.
    for (int i = 0; i < pair_count; i++)
    {
        if (locked[pairs[i].winner][pairs[i].loser] == true)
        {
            detect_winner[pairs[i].winner] = true;

            for (int j = 0; j < pair_count; j++)
            {
                if (locked[pairs[j].winner][pairs[i].loser] == true)
                {
                    detect_winner[pairs[i].winner] = false;
                }
            }
        }
    }

    // Find which candidate is locked in the most over other candidates.
    for (int i = 0; i < candidate_count; i ++)
    {
        if (detect_winner[i] > max)
        {
            max = detect_winner[i];
        }
    }

    // Print the winner(s).
    for (int i = 0; i < candidate_count; i++)
    {
        if (detect_winner[i] == max)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

bool helper(int current, int check, bool tracker[])
{
    // Check if the losing candidate (check) is preffered over another candidate, who is preffered over another candidate, who is preffered over another candidate, and so on and so on.
    // If in this cycle of checking we ever return to current (the winning candidate AKA candidate of pairs[i].winner in main) then theres a cycle, return true.
    if (current == check)
    {
        return true;
    }
    else
    {
        for (int i = 0; i < pair_count; i++)
        {
            if (tracker[i] == false && locked[pairs[i].winner][pairs[i].loser] == true)
            {
                tracker[i] = true;
                return helper(pairs[i].loser, check, tracker);
            }
            else
            {
                continue;
            }
        }
    }
    return false;
}
