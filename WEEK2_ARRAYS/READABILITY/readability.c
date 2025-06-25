#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
void print_grade(float index);

int main(void)
{
    // PROMPT THE USER FOR SOME TEXT
    string prompt = get_string("Text: ");

    // COUNT THE NUMBER OF LETTERS, WORDS, AND SENTENCES IN THE TEXT
    int letters = count_letters(prompt);
    int words = count_words(prompt);
    int sentences = count_sentences(prompt);

    // IF THERE ARE NO WORDS
    if (words == 0)
    {
        printf("No words to evaluate!");
        return 1;
    }

    /count_letters_words_sentences
    print_grade(index);

    return 0;
}

int count_letters(string text)
{
    int letters = 0;

    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }

    return letters;
}

int count_words(string text)
{
    int words = 0;

    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (isspace(text[i]))
        {
            words++;
        }
    }
    words++;

    return words;
}

int count_sentences(string text)
{
    int sentences = 0;

    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }

    return sentences;
}

void print_grade(float index)
{
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", (int) round(index));
    }
}
