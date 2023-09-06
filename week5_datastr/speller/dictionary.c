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
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 100;

// Hash table
node *table[N];

//global variables
int count = 0;
int bkt = 0;
node *tmp = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    bkt = 0; //initialize bucket
    //find out which bucket it's in
    bkt = hash(word);

    //iterate through the list in that bucket
    tmp = table[bkt];
    while (tmp != NULL)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
        else
        {
            tmp = tmp->next;
        }

    }

    return false; //word was never found, return false

}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int sum = 0;

    //sum ASCII values of the string (all uppercase)
    for (int i = 0; i < strlen(word); i++)
    {
        sum += toupper(word[i]);
    }

    sum /= N;
    return sum;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    bkt = 0; //initialize bucket
    //fill the table with NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    //open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    //load strings one at a time into hash table
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF) //keep scanning until end of file
    {
        //find some memory for a new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        //place current word in the node
        strcpy(n->word, word);

        //place this node in the appropriate hash bucket and append to start of list
        bkt = hash(word);
        n->next = table[bkt]; //n points to start of list (first one will be NULL)
        table[bkt] = n; //now start of list points to n
        count++;

    }

    //close file and return bool
    fclose(file);
    return true; //false was returned if either file or n was NULL
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return count; //zero will be returned if no words have been counted yet
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *point = 0; //ensure something is always pointing to start of the list
    node *hold = 0;
    int freed = 0; //counts how many linked lists have been freed
    //iterate through each bucket
    for (int i = 0; i < N; i++)
    {
        point = table[i];
        hold = table[i];

        //iterate until end of list
        while (hold != NULL)
        {

            //move point to next node, hold stays to clear previous node
            point = point->next;
            free(hold);
            hold = point;
        }

        //one linked list is freed, add to counter. Need N total lists to be freed
        if (hold == NULL)
        {
            freed++;
        }

    }

    //if all lists have been freed
    if (freed == N)
    {
        return true;
    }

    return false;
}
