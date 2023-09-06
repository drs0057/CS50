# TODO
from cs50 import get_int

height = get_int("Height: ")

# Checks that input is between 1 and 8
while height < 1 or height > 8:
    height = get_int("Height: ")


def main():

    for i in range(height):

        print_spaces(i)
        print_hashes(i)
        print("  ", end="")
        print_hashes(i)
        print()


# Prints appropriate amount of spaces
def print_spaces(i):

    for n in range(height - i - 1):
        print(" ", end="")

# Prints appropriate amount of hashes
def print_hashes(i):

    for n in range(i + 1):
        print("#", end="")


main()