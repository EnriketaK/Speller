// Implements a dictionary's functionality

#include <stdbool.h>
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 300000; //or 2x the dictionary nr?

int counter = 0;
bool isloaded;
int hashcode;
int result;


// Hash table
node *table[N]; //=Null?
char string[LENGTH + 1];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    const int length = strlen(word); // get the length of the text
    char *lower = malloc(length + 1); // allocate 'length' bytes + 1 (for null terminator) and cast to char*
    lower[length] = 0; // set the last byte to a null terminator

    // copy all character bytes to the new buffer using tolower
    for (int i = 0; i < length; i++)
    {
        lower[ i ] = tolower(word[i]);
    }

    hashcode = hash(lower);

    for (node *current = table[hashcode]->next; current != NULL; current = current->next)
    {
        result = strcasecmp(lower, current->word);
        if (result == 0)
        {
            free(lower);
            return true;
        }
    }
    free(lower);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int sum = 0;
    for (int j = 0; word[j] != '\0'; j++)
    {
        sum += word[j];
    }
    return sum % N;
}//?????? //why doesnt this one work either?

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *inptr = fopen(dictionary, "r"); //fclose
    if (inptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not open %s.\n", dictionary);
        isloaded = false;
        return false;
    }

    node *n = NULL;

    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node)); //for the worst case *N?
        table[i]->next = NULL;
    }

    while (!feof(inptr))
    {
        fscanf(inptr, "%s", string);

        n = malloc(sizeof(node));
        if (n == NULL)
        {
            free(n);
            isloaded = false;
            return false;
        }

        //fcanf moves cursor further
        //loop doesnt allow last word to be repeated because of empty newline in dictionary
        if (!feof(inptr))
        {
            strcpy(n->word, string);
            counter++;
            hashcode = hash(n->word);
            if (table[hashcode]->next == NULL)
            {
                node *list = n;
                table[hashcode]->next = list;
                n->next = NULL;

            }
            else
            {
                //malloc?
                n->next = table[hashcode]->next; //or no need?
                node *list = n; //which
                table[hashcode]->next = list;
            }
        }
        else
        {
            free(n);
        }
    }
    fclose(inptr);
    isloaded = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (isloaded)
    {
        return counter;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int j = 0; j < N; j++)
    {
        if (table[j]->next != NULL)
        {
            while (table[j]->next->next != NULL)
            {
                node *tmp = table[j]->next->next;
                free(table[j]->next);
                table[j]->next = tmp;
            }
            free(table[j]->next);
            free(table[j]);
        }
        else
        {
            free(table[j]);
        }
    }
    return true;
}
