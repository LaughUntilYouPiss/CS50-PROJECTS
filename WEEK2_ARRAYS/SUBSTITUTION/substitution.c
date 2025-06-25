#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // IF TOO MANY ARGUMENTS OR LACK OF KEY
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];
    int length = strlen(key);

    // IF INVALID KEY LENGTH
    if (length != 26)
    {
        printf("key must contain 26 characters\n");
        return 1;
    }

    for (int i = 0; i < length; i++)
    {
        // IF DUPLICATE CHARACHTERS
        for (int j = 0; j < length && j != i; j++)
        {
            if (key[i] == key[j])
            {
                printf("key must not contain duplicate characters\n");
                return 1;
            }
        }

        // IF INVALID CHARACHTER
        if (!isalpha(key[i]))
        {
            printf("key must contain letters only\n");
            return 1;
        }
    }

    // PROMPT FOR PLAIN TEXT
    string plain = get_string("plaintext: ");
    printf("ciphertext: ");

    // ENCRYPT PLAIN TEXT USING KEY
    for (int i = 0; plain[i] != '\0'; i++)
    {
        if (isalpha(plain[i]))
        {
            int index = tolower(plain[i]) - 'a';
            char cichar = isupper(plain[i]) ? toupper(key[index]) : tolower(key[index]);
            printf("%c", cichar);
        }

        else
        {
            printf("%c", plain[i]);
        }
    }

    printf("\n");
    return 0;
}
