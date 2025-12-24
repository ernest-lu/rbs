import random
import datetime

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
    Generates all possible bracket sequences of a given length.
    """
    if length == 0:
        return [""]

    sequences = []
    for i in range(2**length):
        bin_str = bin(i)[2:].zfill(length)
        sequence = bin_str.replace("0", "(").replace("1", ")")
        sequences.append(sequence)
    return sequences


def main():
    """
    Generates a dataset of bracket sequences and writes them to a file.
    """
    num_sequences = 300
    length = 10

    all_sequences = generate_all_sequences(length)

    if num_sequences > len(all_sequences):
        raise ValueError(
            "Number of requested sequences is greater than possible sequences."
        )

    dataset = random.sample(all_sequences, num_sequences)

    out_file_name = "test_data.txt"

    with open(out_file_name, "w") as f:
        for sequence in dataset:
            valid = is_valid(sequence)
            f.write(f"{sequence} {valid}\n")


if __name__ == "__main__":
    main()
