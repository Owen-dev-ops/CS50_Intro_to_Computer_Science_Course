// Encrypts messages using a substitution cipher.

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

string encipher(string k, string p);
bool isvalid(string s);

int main(int argc, string argv[])
{
    // Make sure users key is valid.
    if (argc != 2)
    {
        printf("Usage: ./substitution 'KEY'\n");
        return 1;
    }
    else if (!isvalid(argv[1]))
    {
        return 1;
    }
    string key = argv[1];

    // Get plaintext from the user.
    string plaintxt = get_string("Plaintext: ");

    // Declare and initiate encipheredtxt. Use encipher function to encipher plaintext.
    string encipheredtxt = encipher(key, plaintxt);

    // prints ciphertext to the user (case of the letters should be preserved)
    printf("ciphertext: %s\n", encipheredtxt);

    // Free allocated memory for encipheredtxt to prevent memory leaks.
    free(encipheredtxt);
}

// Checks to ensure argv[1] is a valid entry. (26 letters long, no digits, no duplicate letters).
bool isvalid(string s)
{
    // Declare and initiate n as the length of s.
    int n = strlen(s);

    // Check to make sure the length of s is 26. If not argv[1] is invalid, return false.
    if (n != 26)
    {
        printf("Key must contain 26 letters.\n");
        return false;
    }

    // Iterate each char in s.
    for (int i = 0; i < n; i++)
    {
        // If any char within string s is not a letter return false.
        if (!isalpha(s[i]))
        {
            printf("Key must only contain letters.\n");
            return false;
        }
        // Compare the char at s[i] to every other char in string s. If any match, return false.
        for (int j = i + 1; j < n; j++)
        {
            if (s[j] == s[i])
            {
                printf("Key must not contain duplicate letters.\n");
                return false;
            }
        }
    }
    return true;
}

string encipher(string k, string p)
{
    // Declare the return variable.
    char *etxt = malloc((strlen(p) + 1) * sizeof(char));
    int n = strlen(p);

    // Iterate through every letter of string p and substitute each letter to its corresponding key letter.
    for (int i = 0; i < n; i ++)
    {
        if (p[i] >= 'A' && p[i] <= 'Z')
        {
            etxt[i] = toupper(k[(p[i] - 65)]);
        }
        else if (p[i] >= 'a' && p[i] <= 'z')
        {
            etxt[i] = tolower(k[(p[i] - 97)]);
        }
        else
        {
            etxt[i] = p[i];
        }
    }
    etxt[n] = '\0';
    return etxt;
}
