// Takes a key and text from the user to enchipher the users text.

#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool only_digits(string s);
char encipher(char a, int x);

int main(int argc, string argv[])
{
    // Make sure the user enters two arguments and make sure the second is a digit.
    if (argc != 2 || !only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // Convert argv[1] from a string to an int.
    int key = atoi(argv[1]);

    // Once the user enters an acceptable key (see above code) ask the user for plaintext.
    string plaintxt = get_string("Plaintext: ");

    // Declare and initiate ciphertxt array here for the for loop below.
    // Add one so we can later cast this char array as a string. (accounting for null terminator)
    char ciphertxt[strlen(plaintxt) + 1];
    memset(ciphertxt, '\0', strlen(plaintxt) + 1);

    // Iterate each char within plaintxt to encipher with the encipher function and add the
    // Enciphered chars to ciphertxt array.
    for (int i = 0, n = strlen(plaintxt); i < n; i++)
    {
        ciphertxt[i] = encipher(plaintxt[i], key);
    }
    ciphertxt[strlen(plaintxt)] = '\0';

    // Print the enciphered message to the user, cast the array ciphertxt as a string to be printed
    // as such.
    printf("ciphertext: %s\n", ciphertxt);
}

// Returns true if a string only contains digits, false otherwise.
bool only_digits(string s)
{
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}

// Rotatates each letter from plaintext string and rotates it in the alphabet "key" times to
// encipher text.
char encipher(char a, int x)
{
    char encipheredchar = '\0';

    if (a >= 'A' && a <= 'Z')
    {
        encipheredchar = ((a - 'A' + x) % 26) + 'A';
    }
    else if (a >= 'a' && a <= 'z')
    {
        encipheredchar = ((a - 'a' + x) % 26) + 'a';
    }
    else
    {
        encipheredchar = a;
    }
    return encipheredchar;
}
