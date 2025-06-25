# LUHN'S ALGORITHM
def IsLuhn(number):
    sum = 0
    lenght = len(number)

    # START WITH THE NUMBER'S SECOND-TO-LAST DIGIT
    for i in range(lenght - 2, -1, -2):
        dup = 2 * int(number[i])
        if dup > 9:
            dup -= 9
        sum += dup

    # ADD THE OTHER DIGITS
    for i in range(lenght - 1, -1, -2):
        sum += int(number[i])

    # CHECK IF THE LAST DIGIT OF THE SUM IS 0
    return sum % 10 == 0


def CardType(number):
    lenght = len(number)

    if number[0] == '4' and lenght in [13, 16]:
        print("VISA")
    elif number[0] == '5' and '1' <= number[1] <= '5' and lenght == 16:
        print("MASTERCARD")
    elif number[0] == '3' and number[1] in ['4', '7'] and lenght == 15:
        print("AMEX")
    else:
        print("INVALID")


def main():
    # PROMPT FOR A VALID CREDIT CARD NUMBER
    while True:
        number = input('Number: ')
        if number.isdigit():
            break

    # CHECK THE LENGHT OF THE NUMBER
    lenght = len(number)
    if lenght not in range(13, 16):
        print("INVALID")

    # CHECK IF THE CARD IS VALID
    if IsLuhn(number):
        CardType(number)  # CHECK THE CARD TYPE
    else:
        print("INVALID")


if __name__ == '__main__':
    main()
