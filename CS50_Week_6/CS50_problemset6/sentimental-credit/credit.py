# Tells user whether the credit card number they entered is valid or not and from which company the card is from.

# Get card number from the user and copy it as a string.
card_no = int(input("Number: "))
card_no_copy = str(card_no)

# Keeps track of the sum of every other digit in card_no starting from the second to last digit.
result_a = 0

# Keeps track of the sum of every other digit in card_no starting from the last digit.
result_b = 0

# Temp variable only used in calculating result_b
temp = 0

# Sum of result_a and result_b
final_result = 0

# Luhn's Algorithm
while card_no > 0:
    result_a = result_a + card_no % 10
    card_no = card_no // 10
    temp = (card_no % 10) * 2
    card_no = card_no // 10
    if temp > 9:
        temp = (temp % 10) + (temp // 10)
    result_b = result_b + temp
    temp = 0
final_result = result_a + result_b

# If the result of Lugn's algorithm ends in zero, check for which company (if any) the card is from
if final_result % 10 == 0:
    # Check which company the card is from and print to user
    if len(card_no_copy) == 15 and int(card_no_copy) // 10000000000000 == 15 or int(card_no_copy) // 10000000000000 == 37:
        print("AMEX")
    elif len(card_no_copy) == 16 and int(card_no_copy) // 100000000000000 >= 51 and int(card_no_copy) // 100000000000000 <= 55:
        print("MASTERCARD")
    elif len(card_no_copy) == 13 and int(card_no_copy) // 1000000000000 == 4 or len(card_no_copy) == 16 and int(card_no_copy) // 1000000000000000 == 4:
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
