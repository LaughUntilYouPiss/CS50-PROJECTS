import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py [file.csv] [file.txt]")
        sys.exit()

    csv_file, dna_file = sys.argv[1], sys.argv[2]

    # TODO: Read database file into a variable
    with open(csv_file, mode='r') as data_file:
        data_reader = list(csv.DictReader(data_file))

    # TODO: Read DNA sequence file into a variable
    with open(dna_file, mode='r') as dna_file:
        dna_reader = dna_file.read()

    # TODO: Find longest match of each STR in DNA sequence
    agatc = longest_match(dna_reader, 'AGATC')
    aatg = longest_match(dna_reader, 'AATG')
    tatc = longest_match(dna_reader, 'TATC')

    tttt = longest_match(dna_reader, 'TTTTTTCT')
    tctag = longest_match(dna_reader, 'TCTAG')
    gata = longest_match(dna_reader, 'GATA')
    gaaa = longest_match(dna_reader, 'GAAA')
    tctg = longest_match(dna_reader, 'TCTG')

    # TODO: Check database for matching profiles
    if len(data_reader[0]) == 4:
        for data in data_reader:
            if [int(data['AGATC']), int(data['AATG']), int(data['TATC'])] == [agatc, aatg, tatc]:
                print(data['name'])
                return

    elif len(data_reader[0]) == 9:
        for data in data_reader:
            if [int(data['AGATC']), int(data['TTTTTTCT']), int(data['AATG']), int(data['TCTAG']),
                    int(data['GATA']), int(data['TATC']), int(data['GAAA']),
                    int(data['TCTG'])] == [agatc, tttt, aatg, tctag, gata, tatc, gaaa, tctg]:
                print(data['name'])
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
