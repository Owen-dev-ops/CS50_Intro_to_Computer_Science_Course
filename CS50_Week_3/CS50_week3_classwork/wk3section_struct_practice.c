// Creating structs practice

#include <cs50.h>
#include <stdio.h>

typedef struct
{
    string name;
    int votes;
} politician;

int main(void)
{
    politician canidate[3];
    const N = sizeof(canidate[]);

    for (i = 0; i < N; i++)
    {
        canidate[i].name = get_string("What is the canidates name?: ");
        canidate[i].votes = get_int("How many votes does this canidate have?: ");
    }

    int highest_vote = 0;
    for (int i = 0; i < N; i++)
    {
        if (canidate[i].votes > highest_vote)
        {
            highest_vote = candiate[i].votes;
        }
    }

    for (int i = 0; i < N; i++)
    {
        if (canidate[i].votes == highest_vote)
        {
            printf("%s\n", canidate[i]);
        }
    }



}
