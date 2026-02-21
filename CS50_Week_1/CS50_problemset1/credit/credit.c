// File calculates if the user's entered credit card number is valid or not as well as which company the card if from.

#include <cs50.h>
#include <stdio.h>
#include <stdbool.h>

// Holds value for the user's card number.
long cardno;

// Holds the sum of the two calculate functions.
long validity;

long calculate_digits_times_two(long a);
long calculate_digits(long b);
string check_company(long c);

int main(void)
{
    // Asks the user for the card number.
    do
    {
        cardno = get_long("Number: ");
    }
    while (cardno < 1);

    validity = calculate_digits_times_two(cardno) + calculate_digits(cardno);

    // If the last digit of validity is zero the card is valid (Thus far before checking for the company associated with the card.
    // Card could still be invalid if there is no company to associate it with).
    if (validity % 10 == 0)
    {
        // Run check_company function and print its result to the user. Finally we'll know if their card is truly valid or not and its company.
        printf("%s\n", check_company(cardno));
    }
    // If the last digit of the variable validity is not zero then the card is invalid, tell the user this.
    else
    {
        printf("INVALID\n");
    }
}

// Function takes every other digit starting from second to last digit of the card number and multiplies it by two.
// If the product is greater than nine when multiplied by two, it is broken down into its individual digits before being added to the rest.
// It then adds these products (digits) together.
long calculate_digits_times_two(long a)
{
    long every_othr_xtwo_no = 0;
    long result = 0;
    bool process = false;

    for (int i = 0; a > 0; i++)
    {
        if (process)
        {
            result = (a % 10) * 2;
            if (result > 9)
            {
                result = (result % 10) + (result / 10);
            }
            every_othr_xtwo_no += result;
            result = 0;
        }
    a /= 10;
    process = !process;
    }
    return every_othr_xtwo_no;
}

// Function aquires the sum of every other digit starting from the last digit of the card number.
long calculate_digits(long b)
{
    // Holds the return value for this function.
    long every_othr_no = 0;

    // Used for a flag within this function.
    bool process = true;

    for (int i = 0; b > 0; i++)
    {
        if (process)
        {
            every_othr_no += (b % 10);
        }
        b /= 10;
        process = !process;
    }
    return every_othr_no;
}

// Finds out which company the users card belongs to. If no association is found with one of the companies the card is invalid.
string check_company(long c)
{

    // Holds c to be used again after checking card length.
    long c_holder = c;

    // Holds the length of the user's card number.
    int card_length = 0;

    for (card_length = 0; c > 0; card_length++)
    {
        c /= 10;
    }
    c = c_holder;

    if (card_length == 15 && (c / 10000000000000 == 34 || c / 10000000000000 == 37))
    {
        return "AMEX";
    }
    else if (card_length == 16 && (c / 100000000000000 >= 51 && c / 100000000000000 <= 55))
    {
        return "MASTERCARD";
    }
    else if ((card_length == 13 || card_length == 16) && (c / 1000000000000 == 4 || c / 1000000000000000 == 4))
    {
        return "VISA";
    }
    else
    {
        return("INVALID");
    }
}
