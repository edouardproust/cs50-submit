def main():
    # Prompt user until the input is a numeric value
    while True:
        card_nb = input("Number: ")
        if not card_nb.isnumeric():
            continue
        # Check card validity and print its type
        if check_luhn_algorythm(card_nb):
            print_type(card_nb)
        else:
            print("INVALID")
        break


def check_luhn_algorythm(card_nb):
    """Check that the input is a valid Credit Card number, based on the Luhn's algorythm. Return True if is a valid number, False otherwise"""
    # Base case: Check length
    if len(card_nb) not in [13, 15, 16]:
        return False

    sum = 0
    # Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
    # `[::-1]` reverses the string and `[1::2]` returns every-two digits, starting from the 2nd digit (index 1)
    for d in card_nb[::-1][1::2]:
        for i in str(int(d)*2):
            sum += int(i)
    # Add the sum to the sum of the digits that weren’t multiplied by 2.
    # `[::2]` returns every-two digits, starting from the 1st digit (index 0)
    for d in card_nb[::-1][::2]:
        sum += int(d)
    # If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
    if str(sum)[1] != "0":
        return False

    return True


def print_type(card_nb):
    """Print the type of the credit card: AMEX, VISA or MASTERCARD"""
    # American Express uses 15-digit numbers and start with 34 or 37
    if len(card_nb) == 15 and int(card_nb[:2]) in [34, 37]:
        print("AMEX")
    # MasterCard uses 16-digit numbers and start with 51, 52, 53, 54, or 55
    elif len(card_nb) == 16 and int(card_nb[:2]) in [51, 52, 53, 54, 55]:
        print("MASTERCARD")
    # Visa uses 13- and 16-digit numbers and start with 4
    elif len(card_nb) in [13, 16] and card_nb[:1] == "4":
        print("VISA")
    else:
        print("INVALID")
    return


if __name__ == "__main__":
    main()
