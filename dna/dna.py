import csv
import sys


def main():
    # TODO: Check for command-line usage:
    # - first argument: name of a CSV file containing the STR counts for a list of individuals
    # - second argument: name of a text file containing the DNA sequence to identify
    if len(sys.argv) != 3:
        print("Usage: python dna.py <CSV file of the STR counts> <TXT file of DNA sequence>")
        print("Example: python dna.py databases/large.csv sequences/5.txt")
        sys.exit(1)

    # TODO: Read database file into a variable
    with open(sys.argv[1]) as file:
        # Get list from file without loop: https://stackoverflow.com/questions/32038776/csv-reader-object-is-not-subscriptable
        profiles = list(csv.reader(file))
        subsequences = profiles[0]  # Get subsequences list from header
        # Remove header: https://stackoverflow.com/questions/37672850/how-can-i-remove-specific-index-from-list-in-python
        del profiles[0]
        del subsequences[0]

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2]) as file:
        sequence = list(csv.reader(file))[0][0]

    # TODO: Find longest match of each STR in DNA sequence
    longuest_matches = []
    for subsequence in subsequences:
        longuest_match = longest_match(sequence, subsequence)
        longuest_matches.append(longuest_match)

    # TODO: Check database for matching profiles
    match = False
    for profile in profiles:
        # Convert values into integers: https://www.geeksforgeeks.org/python-converting-all-strings-in-list-to-integers/
        profile_str_counts = [eval(i) for i in profile[1:]]
        if profile_str_counts == longuest_matches:
            match = profile[0]
            break
    if match != False:
        print(match)
    else:
        print("No match")

    exit(0)


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
