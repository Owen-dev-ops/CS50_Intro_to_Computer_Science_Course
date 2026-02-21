#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    //Ask the user and get the height they'd like the pyramid to be.
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1);

    //As long as the variable pheight_check (Short for Pyramid Height Check) is less than the height the user would like the pyramid to be; repeat the loop.
    for(int pheight_check = 0; pheight_check < height; pheight_check++)
    {
        //As long as i is less than the Pyramids height (-1 to prevent extra spaces) print spaces to the screen.
        for (int i = pheight_check; i < height - 1; i++)
        {
            printf(" ");
        }
        //As long as j is less than or equal to the pyramid height check print blocks (#) to the screen.
        for (int j = 0; j <= pheight_check; j++)
        {
            printf("#");
        }
        //Go to next row to start the next layer of the pyramid.
        printf("\n");
    }
}





