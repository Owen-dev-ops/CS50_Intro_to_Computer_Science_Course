// Prints the variables of an array. The array holds five variables starting from one.
// Each consecutive varaiable is two times the previous one

#include <cs50.h>
#include <stdio.h>

int const SIZE = 5;

int main (void)
{
    int array[5];
    array[0] = 1;

    for(int i = 1; i < SIZE; i++)
    {
        array[i] = array[i - 1] * 2;
    }

    for(int i = 0; i < SIZE; i++)
    {
        printf("Element %i: %i\n", i, array[i]);
    }
}
