// Check if a lowercase string's characters are in alphabetical order. If yes print yes and if no print no.

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Argc = argument count. Argv = an array of strings representing the arguments.
// Allows program to accept command-line arguments.
int main(int argc, string argv[])
{
    // If arguments in terminal exceeds two produce the following error message.
    if (argc != 2)
    {
        printf("Please provide a word.\n");
        return 1;
    }
    // String s = the second string entered in command-line.
    string s = argv[1];

    // Iterate s[]'s variables.
    for (int i = 0, n = strlen(s); i < n - 1; i++)
    {
        // If a char in s[] is not a letter produce following error message.
        if (!isalpha(s[i]))
        {
            printf("Invalid entry, please only enter lowercase letters.\n");
            return 2;
        }
        // Check to make sure this char is in alphabetical order within s[]. If not tell the user this.
        else if (s[i] > s[i + 1])
        {
            printf("No\n");
            return 0;
        }
    }
    printf("Yes\n");
}

