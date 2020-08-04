from cs50 import get_string


def main():
    card = get_string("Number: ")
    if not verify(card):
        print("INVALID")
    else:
        print(check(card))


# find out if the card number is valid
def verify(card):

    sum = 0
    for i in range(len(card)):
        pos = len(card) - 1 - i

        # even
        if i % 2 == 0:
            sum += int(card[pos])
            continue

        # odd
        mult = str(2 * int(card[pos]))
        for j in range(len(mult)):
            sum += int(mult[j])

    return (sum % 10 == 0)


# find the provider
def check(card):

    # defines vars
    l = len(card)
    num = int(card[0]) * 10 + int(card[1])

    # checking
    if l == 15 and num in [34, 37]:
        return "AMEX"
    elif l == 16 and num in [51, 52, 53, 54, 55]:
        return "MASTERCARD"
    elif l in [13, 16] and card[0] == "4":
        return "VISA"
    else:
        return "INVALID"


main()
