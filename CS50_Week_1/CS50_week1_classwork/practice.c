#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int x = get_int("What is x?: ");
    switch(x)
    {
        case 5:
        printf("x = 1\n");
        case 2:
        printf("x = 2\n");
        case 3:
        printf("x = 3\n");
        case 1:
        printf("x = 7\n");
        default:
        printf("Sorry!\n");
    }
}
