#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// CHECK IF THE STRING IS A NUMBER
bool StringIsDigit(string S);

// IMPLEMENT LUHN’S ALGORITHM
bool IsLuhn(string number);

// CHECK CARD TYPE
void CardType(string number);

int main(void)
{
    string number;
    int length;

    // PROMPT FOR A VALID CREDIT CARD NUMBER
    do
    {
        number = get_string("Number: ");
        length = strlen(number);
    }
    while (!(StringIsDigit(number)));

    // CHECK THE LENGTH OF THE NUMBER
    if (length > 16 || length < 13)
    {
        printf("INVALID\n");
        return 0;
    }

    // CHECK IF THE CARD IS VALID
    if (IsLuhn(number))
    {
        CardType(number);
        return 0;
    }

    else
    {
        printf("INVALID\n");
        return 0;
    }

    return 0;
}

bool StringIsDigit(string number)
{
    // ITERATE THOROUGH THE STRING
    for (int i = 0, length = strlen(number); i < length; i++)
    {
        // CHECH IF EACH CHARACTER IS A DIGIT
        if (!isdigit(number[i]))
        {
            return false;
        }
    }
    return true;
}

bool IsLuhn(string number)

{
    int sum = 0, length = strlen(number);

    // START WITH THE NUMBER'S SECOND-TO-LAST DIGIT
    for (int i = length - 2; i >= 0; i -= 2)
    {
        int dup = 2 * (number[i] - '0');
        if (dup > 9)
        {
            dup -= 9;
        }
        sum += dup;
    }

    // ADD THE OTHER DIGIT
    for (int i = length - 1; i >= 0; i -= 2)
    {
        sum += (number[i] - '0');
    }

    // CHECK IF THE LAST DIGIT OF THE SUM IS 0
    if (!(sum % 10))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CardType(string number)
{
    int length = strlen(number);

    if (number[0] == '4' && (length == 13 || length == 16))
    {
        printf("VISA\n");
    }

    else if (number[0] == '5' && number[1] <= '5' && number[1] >= '1' && length == 16)
    {
        printf("MASTERCARD\n");
    }

    else if (number[0] == '3' && (number[1] == '4' || number[1] == '7') && length == 15)
    {
        printf("AMEX\n");
    }

    else
    {
        printf("INVALID\n");
    }
}
