#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Declare pheight (short for pyramid height) for later use in code.
    int pheight;

    // Ask and get the number from the user for how high the pyramid should be. User must enter a number greater than 0.
    do
    {
        pheight = get_int("How tall should the pyramid be?\n");
    }
    while (pheight < 1 || pheight > 8);

    // As long as hcheck (short for height check) is less than the height the user would like, keep going through this loop.
    for (int hcheck = 0; hcheck < pheight; hcheck++)
    {
        //As long as i is less than the height the pyramid should be (-1 to prevent an extra space) keep printing spaces.
        for (int i = hcheck; i < pheight - 1; i++)
        {
            printf(" ");
        }
        // As long as j is less than or equal to the height checking variable continue to print #s.
        for (int j = 0; j <= hcheck; j++)
        {
            printf("#");
        }
        // Print two spaces to creat the gap in the middle of the pyramid.
        printf("  ");
        // As long as k is less than or equal to the height checker print #s to make the opposite side of the pyramid.
        for (int k = 0; k <= hcheck; k++)
        {
            printf("#");
        }
        // Make cursor go to next line so the main loop can begin again, creating the next row the pyramid.
        printf("\n");
    }
}
