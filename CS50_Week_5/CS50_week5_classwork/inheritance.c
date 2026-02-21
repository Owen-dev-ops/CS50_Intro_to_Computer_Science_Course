// Simulate genetic inheritance of blood type.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles.
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
} person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // Seed random number generator.
    srand(time(0));

    // Create a new famility with three generations.
    person *p = create_family(GENERATIONS);

    // Print family tree of blood types.
    print_family(p, 0);

    // Free memory.
    free_family(p);
}

// Create a new individual with 'generations'
person *create_family(int generations)
{
    // Allocate memory for new person.
    person *new_person = malloc(sizeof(person));
    if(new_person == NULL)
    {
        return NULL;
    }

    // If there are still generations left to create.
    if (generations > 1)
    {
        // Create two new parents for current person by recursively calling create_family.
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        // Set parent pointers for current person.
        new_person->parents[0] = parent0;
        new_person->parents[1] = parent1;

        // Randomly assing current persn's alleles based of their parents.
        new_person->alleles[0] = parent0->alleles[rand() % 2];
        new_person->alleles[1] = parent1->alleles[rand() % 2];
    }

    // If there are no generations left to create
    else
    {
        // Set parent pointers to NULL.
        new_person->parents[0] = NULL;
        new_person->parents[1] = NULL;

        // Randomly assign alleles.
        new_person->alleles[0] = random_allele();
        new_person->alleles[1] = random_allele();
    }

    // Return newly created person.
    return new_person;
}

void free_family(person *p)
{
    // Handle base case.
    if (p == NULL)
    {
        return;
    }

    // Free parents recursively.
    free_family(p->parents[0]);
    free_family(p->parents[1]);

    // Free child.
    free(p);
}






char random_allele()
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 0;
    }
}
