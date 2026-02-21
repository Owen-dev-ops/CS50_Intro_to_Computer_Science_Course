// Implements a dictionary's functionality

#include "dictionary.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

bool insert(char *word_to_insert, int hash_code);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Declare and set number of buckets in hash table.
const unsigned int N = 997;

// Constant used for calculating hash codes.
int HASH_CODE_CONSTANT = 827;

// Hash table
node *table[N];

// Keeps track of the number of words we load into the hash table from the dictionary.
int num_of_words_loaded = 0;

// Keeps track of whether loaded has been run yet.
bool loaded = false;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Ensure each letter from word is lowercase so the hash function works properly.
    int n = strlen(word);
    char lcase_word[n + 1];
    for (int i = 0; i < n; i++)
    {
        lcase_word[i] = tolower(word[i]);
    }
    lcase_word[n] = '\0';

    // Hash word.
    int hash_code = hash(lcase_word);

    // Declare and initiate table pointer.
    struct node *scanner = table[hash_code];

    // If scanner points to nothing we know the word isn't in the dictionary.
    if (scanner == NULL)
    {
        return false;
    }
    // Check each node for the word, if we reach the end of the linked list and words still not
    // there we know it's not in the dictionary.
    else
    {
        while (strcasecmp(scanner->word, lcase_word) != 0)
        {
            if (scanner->next == NULL)
            {
                return false;
            }
            else
            {
                scanner = scanner->next;
            }
        }
        return true;
    }
}

// Hashes word to a number.
unsigned int hash(const char *word)
{
    int hash_code = 0;
    int exponent = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        hash_code += word[i] * (int) pow(HASH_CODE_CONSTANT, exponent);
        exponent++;
    }
    return hash_code % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Letter will hold characters from the words in the given dictionary.
    char letter = '\0';

    // Not to be confused with word[] at the top of this file.
    // Holds each word from the given dictionary to then be put into the hash table.
    char load_word[LENGTH];

    // Index tells us which character we are at in load_word[].
    int index = 0;

    // Set all pointers in table to NULL to start.
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open the dictionary file.
    FILE *source = fopen(dictionary, "r");

    // Return false if dictionary cannot be loaded.
    if (source == NULL)
    {
        return false;
    }

    // Read each word in the file.
    while (fread(&letter, sizeof(char), 1, source) != 0)
    {
        // As long as we have not reached the end of the string keep adding charcters to it.
        if (letter != '\n')
        {
            load_word[index] = letter;
            index++;
        }
        // If word is complete, hash it.
        else
        {
            // Turn word into a string. Reset index to be used for placing next word into the hash
            // table.
            load_word[index] = '\0';
            index = 0;
            num_of_words_loaded++;

            // Put word into hash table.
            if (!insert(load_word, hash(load_word)))
            {
                return false;
            }
        }
    }
    // Close the dictionary file.
    fclose(source);

    // Dictionary loaded.
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (loaded)
    {
        return num_of_words_loaded;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Linked list pointer.
    node *scanner = NULL;

    // Pointer to point at node were freeing.
    node *clear = NULL;

    // Iterate through each index in the hash table.
    for (int i = 0; i < N; i++)
    {
        // If there's no list to free at this index go to next index. Else go through the list here
        // and free each node.
        if (table[i] == NULL)
        {
            continue;
        }
        else
        {
            scanner = table[i];
            while (scanner->next != NULL)
            {
                clear = scanner;
                scanner = scanner->next;
                free(clear);
            }
            free(scanner);
        }
    }
    return true;
}

bool insert(char *word_to_insert, int hash_code)
{
    // Scanning variable used to traverse the table.
    struct node *scanner = NULL;

    // If there are no nodes connected to the hash_code's index in the table, insert the new word
    // here.
    if (table[hash_code] == NULL)
    {
        table[hash_code] = malloc(sizeof(struct node));
        if (table[hash_code] == NULL)
        {
            return false;
        }
        strcpy(table[hash_code]->word, word_to_insert);
        table[hash_code]->next = NULL;

        return true;
    }

    // If there are nodes already located at the hash_code index, traverse the linked list there
    // until one of the nodes next field points to NULL. Then add a node there, copy the new word
    // into the word field and set this nodes next field to NULL.

    scanner = table[hash_code];

    while (scanner->next != NULL)
    {
        scanner = scanner->next;
    }
    scanner->next = malloc(sizeof(struct node));
    if (scanner->next == NULL)
    {
        return false;
    }
    scanner = scanner->next;

    strcpy(scanner->word, word_to_insert);
    scanner->next = NULL;

    return true;
}
