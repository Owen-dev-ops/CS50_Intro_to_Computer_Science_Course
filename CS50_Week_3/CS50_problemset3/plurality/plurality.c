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
} candidate;

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
    // Iterate over each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // If the name the voter entered matches one of the candidates names add 1 to the candidates
        // vote total.
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    // If the name the voter entered dosen't match any candidate, return false. No such candidate
    // exist.
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Initialize and assign 0 to most variable. Keeps track of the highest number of votes within
    // the election.
    int most = 0;

    // Iterate through each candidate.
    for (int i = 0; i < candidate_count; i++)
    {
        // Keep track of the highest number of votes.
        if (candidates[i].votes >= most)
        {
            most = candidates[i].votes;
        }
    }

    // Iterate through the candidates.
    for (int i = 0; i < candidate_count; i++)
    {
        // Print the name of any candidate who got the most amount of votes. Can be multiple if
        // there's a tie.
        if (candidates[i].votes == most)
        {
            printf("%s\n", candidates[i].name);
        }
    }
    return;
}
