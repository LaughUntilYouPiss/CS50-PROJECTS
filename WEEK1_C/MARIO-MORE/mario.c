#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height = get_int("How tall do you want the pyramids to be? ");
    while (height <= 0 || height > 8)
    {
        // PROMPT FOR HEIGHT UNTIL A VALID NUMBER IS ENTERED
        height = get_int("Please type a number between 0 and 8 ");
    }

    // PRINT THE PYRAMID
    for (int i = 0; i < height; i++)
    {
        for (int j = height; j > i + 1; j--)
        {
            printf(" ");
        }

        // PRINT THE LEFT SIDE
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        // PRINT THE GAP
        printf("  ");

        // PRINT THE RIGHT SIDE
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }

        printf("\n");
    }
}
