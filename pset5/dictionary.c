/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

#define LENGTH 45

// DJB2 Hash Function prototype - props to Dan Bernstein 
unsigned long hash(const char *str);

// Nodes for hash table
struct node {
    char ldWord[LENGTH + 1];
    struct node* next;
};

// Counter for loaded words
int hashSizePtr;
int loadCountPtr;

// Load confirmation flag
bool loadConfirm;

// Pointer to hash table
struct node** hashTabPtr;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // Transform word to lowercase
    int len = strlen(word);
    char lowered[len + 1];
    for( int i = 0; i <= len; i++ )
    {
        lowered[i] = tolower(word[i]);
    }
    int hashSize = hashSizePtr;

    // Hash word
    int hashKey = hash(lowered) % hashSize;
    
    // Temporary pointer to bucket
    struct node* temp;
    temp = hashTabPtr[hashKey];
    
    // Look up in bucket
    while( temp != NULL )
    {
        // If the word is found, return true
        if( strcmp( temp->ldWord, lowered ) == 0)
        {
            return true;
        }
        
        // Word not found, look in next node
        else
        {
            temp = temp->next;
        }
    }
    
    // If not found, return false
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Open chosen dictionary
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        return false;
    }

    // Count words in dictionary - needed to determine the size of our hashtable
    char buffer[LENGTH + 1];
    int wordCount = 0;
    while(fgets(buffer, LENGTH + 1, fp) != NULL)
    {
        wordCount++;
    }
    int hashSize = wordCount;

     // Create hash table and initialize it
    struct node* hashTab[hashSize];
    for(int i = 0; i < hashSize; i++)
    {
        hashTab[i] = NULL;
    }
    
    // Reset cursor position to beginning of file
    fseek(fp, 0, SEEK_SET);

    // Load all dictionary words iteratively, and hash each one of them
    int loadCount = 0;
    while(fgets(buffer, LENGTH + 2, fp) != NULL)
    {
        // Create new node
        struct node* newNode;
        if( ( newNode = malloc ( sizeof ( struct node ) ) ) == NULL )
        {
            return false;
        }
        buffer[ strcspn( buffer, "\n" ) ] = 0;
        strcpy(newNode->ldWord, buffer);
        newNode->next = NULL;
        
        // Pass word into hashing function
        int hashKey = ( hash( newNode->ldWord) ) % hashSize;
        
        // If bucket is empty, link new node
        newNode->next = hashTab[hashKey];
        hashTab[hashKey] = newNode;
        
        // Increase word count for later use
        loadCount++;
    }
    
    // Create a pointer table to the different words
    hashTabPtr = malloc( sizeof(struct node) * hashSize);
    for(int i = 0; i < hashSize; i++)
    {
        hashTabPtr[i] = hashTab[i];
    }
    
    // Set load confirmation to true (global flag for other functions)
    loadConfirm = true;
    
    loadCountPtr = loadCount;
    hashSizePtr = wordCount;
    
    fclose(fp);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if(loadConfirm)
    {
        int loadCount = loadCountPtr;
        return loadCount;
    }
    
    return 0;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    int hashSize = hashSizePtr;
    // NEED TO REVISE
    struct node *nodeDlt, *temp;
    
    for(int i = 0; i < hashSize; i++)
    {
        nodeDlt = hashTabPtr[i];
        
        while(nodeDlt != NULL)
        {
            temp = nodeDlt;
            nodeDlt = nodeDlt->next;
            free(temp);
        }
    }
    
    free(hashTabPtr);
    
    return true;
}

/**
 * DJB2 Hash Function - props to Dan Bernstein 
 */
unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}