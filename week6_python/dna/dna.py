import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: ./dna.py databases/FILE.csv sequences/FILE.txt")
        sys.exit(1)

    # TODO: Read database file into a variable
    database = []
    with open(sys.argv[1], "r") as file1:
        hold = csv.DictReader(file1)

        for row in hold:
            database.append(row)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as file2:
        sequence = file2.read()

    # TODO: Find longest match of each STR in DNA sequence

    # Dictionary to store STRs and their longest runs
    STRs = {}

    # Store the headers in a list, remove the first item "names"
    headers = list(database[0].keys())
    headers.pop(0)

    # Iterate through list and store longest runs
    for item in headers:

        STRs[item] = longest_match(sequence, item)

    # TODO: Check database for matching profiles

    for row in database:

        count = 0

        for header in headers:

            if int(row[header]) == STRs[header]:

                count += 1

        # Now see if there is a complete match

        if len(headers) == count:

            print(f"{row['name']}")

            return

    print("No match")

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
