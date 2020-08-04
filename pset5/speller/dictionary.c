// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10000;
// Number of the words in the dictionary
unsigned int words = 0;

// Hash table
node *table[N];

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char w[LENGTH + 1];
    // range over words
    while (fscanf(file, "%s", w) != EOF)
    {

        // construct the node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, w);
        n->next = NULL;

        // obtain hash value, location
        int index = hash(w);

        // if first insert
        if (table[index] == NULL)
        {
            // first value
            table[index] = n;
        }
        else
        {
            // insert the value
            n->next = table[index];
            table[index] = n;
        }
        words++;
    }

    // done
    fclose(file);
    return true;
}

// Hashes word to a number.
unsigned int hash(const char *word)
{
    int h = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        // sum of the letters
        h += tolower(word[i]);
    }
    return (h % N);
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // hash word
    unsigned int index = hash(word);

    // range over linked list
    for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            // word found
            return true;
        }
    }

    // word not found in the dictionary
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // free each liked list
    for (unsigned int i = 0; i < N; i++)
    {
        // defines pointers
        node *head = table[i];
        node *cursor = head;
        node *tmp = head;

        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}