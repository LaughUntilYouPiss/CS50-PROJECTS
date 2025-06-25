// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 257;
int word_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash the word to obtain its hash value
    int c = hash(word);

    // Search the hash table at the location specified by the word’s hash value
    node *ptr = table[c];
    while (ptr != NULL)
    {
        if (strcasecmp(ptr->word, word) == 0)
        {
            // Return true if the word is found
            return true;
        }

        ptr = ptr->next;
    }

    // Return false if no word is found
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 0;

    for (int i = 0; i < 3 && word[i] != '\0'; i++)
    {
        hash += toupper(word[i]) * (i + 1);
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    if (dictionary == NULL)
    {
        printf("Error: No dictionary file provided.\n");
        return false;
    }

    // INITIALIZE HASH TABLE TO NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open the dictionary file
    FILE *source = fopen(dictionary, "r");

    if (source == NULL)
    {
        printf("Could not open file\n");
        return false;
    }

    char word[LENGTH + 1];

    // Read each word in the file
    while (fscanf(source, "%s", word) != EOF)
    {
        // Create space for a new hash table node
        node *new_word = malloc(sizeof(node));

        if (new_word == NULL)
        {
            printf("Could not allocate memory.\n");
            fclose(source);
            unload();
            return false;
        }

        // Copy the word into the new node
        strcpy(new_word->word, word);

        // INCREASE THE SIZE
        word_count++;

        // Hash the word to obtain its hash value
        unsigned int c = hash(word);

        // Insert the new node into the hash table (using the index specified by its hash value)
        new_word->next = table[c];
        table[c] = new_word;
    }

    // Close the dictionary file
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *temp, *cursor;

    // ITERATE OVER EVERY LINKED LIST IN THE HASH TABLE
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];

        while (cursor != NULL)
        {
            temp = cursor;         // KEEP TRACK OF THE CURRENT NODE
            cursor = cursor->next; // MOVE TO THE NEXT NODE
            free(temp);            // DELETE THE CURRENT NODE
        }

        table[i] = NULL;
    }

    word_count = 0;
    return true;
}
