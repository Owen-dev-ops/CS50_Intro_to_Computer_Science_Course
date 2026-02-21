# Gives user minimum number of coins needed to make change for amount entered.

# Get user input as a float, must not be negative. If it is not a float or is negative, reprompt.
while True:
    try:
        change = float(input("Change: "))
        if change > 0.00:
            break
        else:
            print("Please enter a valid change amount. EX: 0.41, 1.25, 9.86")
    except ValueError:
        print("Please enter a valid change amount. EX: 0.41, 1.25, 9.86")

# Converts users entry from being formatted in dollars to cents. Prevents floating point precision issues.
change = round(change * 100)

# Holds how many coins we need to make change.
coins = 0

# While change has yet to be fully made calculate how much we need.
while change > 0:
    # Quarters
    if change >= 25:
        change = change - 25
        coins += 1
    # Dimes
    elif change >= 10:
        change = change - 10
        coins += 1
    # Nickels
    elif change >= 5:
        change = change - 5
        coins += 1
    # Pennies
    elif change >= 1:
        change = change - 1
        coins += 1

print(coins)
