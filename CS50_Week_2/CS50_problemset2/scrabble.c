// Scrabble. Collects a word from two competing users, using the scrabble scoring system each word is scored.
// Then the code prints who won to the screen.

#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int score(string s);

int main(void)
{
    // Aquire both players word.
    string pone = get_string("Player One: ");
    string ptwo = get_string("Player Two: ");

    // Use the scoring function to compare the users scores and print who won accordingly.
    if (score(pone) > score(ptwo))
    {
        printf("Player 1 wins!\n");
    }
    else if (score(pone) < score(ptwo))
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

// Scores a word.
int score(string s)
{
    // Delcare and assign return variable.
    int x = 0;

    // Declare an array for the point values and assign them.
    int scoringrules[128] = {0};

    scoringrules['A'] = scoringrules['a'] = 1;
    scoringrules['B'] = scoringrules['b'] = 3;
    scoringrules['C'] = scoringrules['c'] = 3;
    scoringrules['D'] = scoringrules['d'] = 2;
    scoringrules['E'] = scoringrules['e'] = 1;
    scoringrules['F'] = scoringrules['f'] = 4;
    scoringrules['G'] = scoringrules['g'] = 2;
    scoringrules['H'] = scoringrules['h'] = 4;
    scoringrules['I'] = scoringrules['i'] = 1;
    scoringrules['J'] = scoringrules['j'] = 8;
    scoringrules['K'] = scoringrules['k'] = 5;
    scoringrules['L'] = scoringrules['l'] = 1;
    scoringrules['M'] = scoringrules['m'] = 3;
    scoringrules['N'] = scoringrules['n'] = 1;
    scoringrules['O'] = scoringrules['o'] = 1;
    scoringrules['P'] = scoringrules['p'] = 3;
    scoringrules['Q'] = scoringrules['q'] = 10;
    scoringrules['R'] = scoringrules['r'] = 1;
    scoringrules['S'] = scoringrules['s'] = 1;
    scoringrules['T'] = scoringrules['t'] = 1;
    scoringrules['U'] = scoringrules['u'] = 1;
    scoringrules['V'] = scoringrules['v'] = 4;
    scoringrules['W'] = scoringrules['w'] = 4;
    scoringrules['X'] = scoringrules['x'] = 8;
    scoringrules['Y'] = scoringrules['y'] = 4;
    scoringrules['Z'] = scoringrules['z'] = 10;

    for (int i = 0, n = strlen(s); i < n; i++)
    {
        x = x + scoringrules[(int)s[i]];
    }
    return x;
}
