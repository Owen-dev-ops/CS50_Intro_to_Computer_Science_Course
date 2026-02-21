// Calculates the approximate grade level needed to comprehend some text.

#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // Declare and initialize variables.
    float letters = 0;
    float words = 0;
    float sentences = 0;

    // Get text from the user.
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text.
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
        else if (text[i] == ' ')
        {
            words++;
        }
        else if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentences++;
        }
    }
    // Detecting words by number of spaces, this prevents the code from not counting the last word in text.
    words++;

    // Calculate reading level.
    float index = 0.0588 * (letters / words * 100) - 0.296 * (sentences / words * 100) - 15.8;
    index = round(index);

    // Print reading level to the user.
    if (index >= 1 && index <= 16)
    {
        printf("Grade %i\n", (int) index);
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Before Grade 1\n");
    }
}
