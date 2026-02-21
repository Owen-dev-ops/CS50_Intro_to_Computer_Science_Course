# Outputs grade level required to read some text entered by the user.

# Get text from the user
text = input("Text: ")

# Tracks the number of letters, words, and sentences in text respectively
letters = 0
words = 0
sentences = 0

# Iterate through each character in text and track how many letters, words, and sentences there are in it.
for i in text:
    # If a letter is detected letters++
    if i.isalpha():
        letters += 1
    # If a space is detected words ++
    elif i == ' ':
        words += 1
    # If ending punctuation is detected sentences++
    elif i == '.' or i == '!' or i == '?':
        sentences += 1

# Add last word in text to words
words += 1

# Use variables to calculate reading level
index = 0.0588 * (letters / words * 100) - 0.296 * (sentences / words * 100) - 15.8
index = round(index)

# Output level to user
if index >= 1 and index <= 16:
    print(f"Grade {index}")
elif index < 1:
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
