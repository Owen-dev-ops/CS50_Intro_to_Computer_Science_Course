#include <cs50.h>
#include <stdio.h>

int change_owed;

int coins = 0;

int main(void)
{
    do
    {
        change_owed = get_int("Change owed: ");
    }
    while (change_owed < 0);

    while (change_owed > 0)
    {
        if (change_owed >= 25)
        {
            change_owed = change_owed - 25;
            coins++;
        }
        else if (change_owed >= 10)
        {
            change_owed = change_owed - 10;
            coins++;
        }
        else if (change_owed >= 5)
        {
            change_owed = change_owed - 5;
            coins++;
        }
        else
        {
            change_owed = change_owed - 1;
            coins++;
        }
    }
    printf("%i\n", coins);
}



