#include <cs50.h>
#include <stdio.h>

int main(void)
{
    string name = get_string("What's your name? ");
    int age = get_int("How old are you? ");
    string number = get_string("What's your number? ");

    printf("Name: %s\nAge: %i\nNumber: %s\n", name, age, number);

}
