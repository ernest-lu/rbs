import random
import datetime
import itertools
from more_itertools import distinct_permutations

random.seed(datetime.datetime.now().timestamp())


def is_valid(sequence):
    """
    Checks if a bracket sequence is valid.
    A sequence is valid if:
    1. The total number of opening brackets equals the total number of closing brackets.
    2. At no point in scanning the sequence from left to right is the number of
       closing brackets greater than the number of opening brackets.
    """
    balance = 0
    for bracket in sequence:
        if bracket == "(":
            balance += 1
        elif bracket == ")":
            balance -= 1
        if balance < 0:
            return False
    return balance == 0


def generate_all_sequences(length):
    """
    Generates all possible bracket sequences of a given length with an equal
    number of opening and closing brackets.
    """
    if length % 2 != 0:
        return []

    half_length = length // 2
    base = ["("] * half_length + [")"] * half_length

    # Using set to store unique permutations
    sequences = set("".join(p) for p in distinct_permutations(base))

    return list(sequences)


def main():
    """
    Generates a dataset of bracket sequences and writes them to a file.
    """
    length = 14

    all_sequences = generate_all_sequences(length)
    random.shuffle(all_sequences)

    train_size = int(len(all_sequences) * 0.875)
    train_data = all_sequences[:train_size]
    test_data = all_sequences[train_size:]

    train_out_file_name = "train_data.txt"
    test_out_file_name = "test_data.txt"

    with open(train_out_file_name, "w") as f:
        for sequence in train_data:
            valid = is_valid(sequence)
            f.write(f"{sequence} {valid}\n")

    with open(test_out_file_name, "w") as f:
        for sequence in test_data:
            valid = is_valid(sequence)
            f.write(f"{sequence} {valid}\n")


if __name__ == "__main__":
    main()
