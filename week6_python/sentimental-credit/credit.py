# TODO

from cs50 import get_string
import re


def main():

    # Gather card number
    card = get_string("Number: ")

    # First check if sum is valid
    if not checksum(card):
        print("INVALID")

    # Define if statements for each card
    # AMEX
    elif len(card) == 15 and (card[:2] == "34" or card[:2] == "37"):
        print("AMEX")

    # MASTERCARD
    elif len(card) == 16 and (card[:2] == "51" or card[:2] == "52" or
                              card[:2] == "53" or card[:2] == "54" or
                              card[:2] == "55"):
        print("MASTERCARD")

    # VISA
    elif (len(card) == 13 or len(card) == 16) and card[:1] == "4":
        print("VISA")

    # Catch all
    else:
        print("INVALID")


def checksum(card):

    # Convert to int
    card = int(card)

    # Gather every second digit starting from 2nd to last and *2; // allows truncatio
    num1 = ((card % 100) // 10) * 2
    num2 = ((card % 10000) // 1000) * 2
    num3 = ((card % 1000000) // 100000) * 2
    num4 = ((card % 100000000) // 10000000) * 2
    num5 = ((card % 10000000000) // 1000000000) * 2
    num6 = ((card % 1000000000000) // 100000000000) * 2
    num7 = ((card % 100000000000000) // 10000000000000) * 2
    num8 = ((card % 10000000000000000) // 1000000000000000) * 2

    # Split the products up by digit
    num1 = ((num1 % 100) // 10) + (num1 % 10)
    num2 = ((num2 % 100) // 10) + (num2 % 10)
    num3 = ((num3 % 100) // 10) + (num3 % 10)
    num4 = ((num4 % 100) // 10) + (num4 % 10)
    num5 = ((num5 % 100) // 10) + (num5 % 10)
    num6 = ((num6 % 100) // 10) + (num6 % 10)
    num7 = ((num7 % 100) // 10) + (num7 % 10)
    num8 = ((num8 % 100) // 10) + (num8 % 10)

    # Sum those digits separately
    sum1 = num1 + num2 + num3 + num4 + num5 + num6 + num7 + num8

    # Now gather every second digit starting from last digit
    # num9 is last digit, num10 is 3rd to last digit, etc.
    num9 = ((card % 10) // 1)
    num10 = ((card % 1000) // 100)
    num11 = ((card % 100000) // 10000)
    num12 = ((card % 10000000) // 1000000)
    num13 = ((card % 1000000000) // 100000000)
    num14 = ((card % 100000000000) // 10000000000)
    num15 = ((card % 10000000000000) // 1000000000000)
    num16 = ((card % 1000000000000000) // 100000000000000)

    # Now add sum1 to all the above digits
    totalsum = sum1 + num9 + num10 + num11 + num12 + num13 + num14 + num15 + num16

    # Check if last digit is a 0 in totalsum
    if (totalsum % 10) == 0:
        return True
    else:
        return False


main()
