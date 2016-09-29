/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Performs a binary search, returning true if value is in array, else false
 */

bool bSearch(int key, int array[], int loLim, int uLim)
{
    
    // Returns true if the value in that position is the key
    if ( array[ (uLim + loLim) / 2 ] == key )
        return true;
    
    // Sets new upper limit for array and calls function again
    if ( array[ (uLim + loLim) / 2 ] > key && uLim != loLim )
    {
        uLim = ( ( (uLim + loLim) / 2 ) - 1 );
        return bSearch(key, array, loLim, uLim);
    }
    
    // Sets new lower limit for array and calls function again    
    if ( array[ (uLim + loLim) / 2 ] < key && uLim != loLim )
    {
        loLim = ( ( (uLim + loLim) / 2) + 1 );
        return bSearch(key, array, loLim, uLim);
    }
    
    return false;
}

/**
 * Returns true if value is in array of n values, else false.
 */

bool search(int value, int values[], int n)
{
    // Declare the upper and lower limits of the array
    int uLim = n-1;
    int loLim = 0;
    
    if ( bSearch(value, values, loLim, uLim) )
    {
        return true;
    }
    
    return false;
}

/**
 * Sorts array of n values.
 */

void sort(int values[], int n)
{
    // Bubble sort loop
    for(int i = 0; i < n; i++)
    {
        for(int j = 1, temp; j < n; j++)
        {
            if(values[j] < values[j-1])
            {
                temp = values[j];
                values[j] = values[j-1];
                values[j-1] = temp;
            }
        }
    }
    
    return;
}