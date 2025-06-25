#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// CALCULATE THE SCRABBLE SCCORE
int score(string word);

int main(void)
{
    // PROMPT FOR THE USER FOR TWO WORDS
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // COMPUTE THE SCORE OF EACH WORD
    int score1 = score(word1), score2 = score(word2);

    // PRINT THE WINNER
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int score(string word)
{
    int score = 0;

    int points[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    for (int i = 0, length = strlen(word); i < length; i++)
    {
        if (isalpha(word[i]))
        {
            score += points[(tolower(word[i]) - 'a')];
        }
    }

    return score;
}
