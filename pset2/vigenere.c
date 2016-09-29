/*
SPECS AND NOTES
* Program takes a keyword and a message, uses a Vigenere cypher to encrypt it, returns encrypted message
* Keyword can be typed in upper / lower case, but it shouldn't make a difference
* Formula c[i] = (p[i] + k[j]) % 26
* If program is entered w/o command line argument, terminates returning 1
* Non-alpha marks must remain unchanged 
*/

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//The numbers used for translation from ASCII to alphabetical
#define UPPER 65
#define LOWER 97


int main(int argc, string argv[])
{

    // Check that a command line argument has been included
    if(argc != 2)
    {
        printf("Error: need <key> argument\n");
        return 1;
    }
    
    // Define keyword k as string
    string k = argv[1];
    int lk = strlen(k);
    
    // Check that keyword contains only letters a-z
    for(int i = 0; i < lk; i++)
    {
        if(! ( isalpha(k[i]) ) )
        {
            printf("Keyword must be made of letters only\n");
            return 1;
        }
    }
    
    // Transform keyword into integer array ranging from 0 to 25
    int k_low[lk];
    
    for(int i = 0; i < lk; i++)
    {
        // Array character is switched to lower case and turned into 0-25 value
        k_low[i] = tolower(k[i]) - 97;
    }
    
    // Get phrase from user, calculate length
    string msg = GetString();
    int lm = strlen(msg);
    
    // Counter for keyword loop
    int j = 0;
    
    // Go through every character of phrase, cypher and raise keyword counter
    for (int i = 0; i < lm; i++)
    {
        // Check if character is alphabetic
        if( isalpha(msg[i]) )
        
        {
            // If character is uppercase, cypher accordingly, then raise keyword count
            if ( isupper(msg[i]) )
            {
                msg[i] = ( ( (msg[i] - UPPER + k_low[j%lk]) % 26) + UPPER );
                j += 1;
            }
    
            // If character is lowercase, cypher accordingly, then raise keyword count
            if ( islower(msg[i]) )
            {
                msg[i] = ( ( (msg[i] - LOWER + k_low[j%lk]) % 26) + LOWER );
                j +=1;
            }
        }
    }
    
    // Print the cyphered message
    for (int i = 0; i < lm; i++)
    {
        printf("%c", msg[i]);
    }

    // Print a single break, exit the program
    printf("\n");
    return 0;

}
