# Identifies an individual based on a dna sample

import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Incorrect Usage; try -> python dna.py \"CSV DNA database file here\" \"Text file with DNA Sequence to identify here\"")
        sys.exit()

    # Read database file into a variable
    dna_database = []
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for row in reader:
            dna_database.append(row)

    # Read DNA sequence file into a variable
    dna_sequence = ''
    with open(sys.argv[2]) as file:
        dna_sequence = file.read()

    # Find longest match of each STR in DNA sequence
    longest_match_database = {
        "AGATC": longest_match(dna_sequence, "AGATC"),
        "TTTTTTCT": longest_match(dna_sequence, "TTTTTTCT"),
        "AATG": longest_match(dna_sequence, "AATG"),
        "TCTAG": longest_match(dna_sequence, "TCTAG"),
        "GATA": longest_match(dna_sequence, "GATA"),
        "TATC": longest_match(dna_sequence, "TATC"),
        "GAAA": longest_match(dna_sequence, "GAAA"),
        "TCTG": longest_match(dna_sequence, "TCTG")
    }

    # Check database for matching profiles

    # Holds name of individual who's dna matches the dna in the given dna_sequence.
    name = "No match"
    # Tracks whether the current persons dna within the dna_database is still matching the given dna_sequence.
    matching = None
    # Scan through each person in our dna_database and then scan through each of the STR counts.
    for row in dna_database:
        # Assume there will be a match.
        matching = True
        for key in row:
            # Skip name and compare the STR counts for this person to the STR counts in the longest_match_database.
            if key == "name":
                continue
            # If every STR count so far matches continue.
            elif int(row[key]) == longest_match_database[key]:
                continue
            # Else there is no match. Move onto checking the next person in the dna_database.
            else:
                matching = False
                break
        # If matching is still true then we have a match. Tell the user who it is.
        if matching:
            name = row["name"]
            print(name)
            return
    # If no match is detected tell the user this.
    print(name)
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
