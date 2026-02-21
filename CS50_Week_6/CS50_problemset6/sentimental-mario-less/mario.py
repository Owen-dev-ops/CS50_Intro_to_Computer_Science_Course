# Creates a half-pyramid out of # blocks (1-8 blocks tall).

# Get height from user (if not between 1-8 re-prompt).
while True:
    try:
        height = int(input("Height: "))
        if height > 0 and height < 9:
            break
        else:
            print("Please enter a number greater than 0 and less than 9")
    except ValueError:
        print("Please enter a number greater than 0 and less than 9")

# Tracks how many spaces we need.
width = height

# Print pyramid.
for i in range(height):
    for j in range(width - 1):
        print(" ", end="")
    width -= 1
    for k in range(i + 1):
        print("#", end="")
    print()
