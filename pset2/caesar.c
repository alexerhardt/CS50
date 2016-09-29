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

    // Check that argument k has been entered correctly
    if(argc != 2)
    {
        printf("Error: need <key> argument\n");
        return 1;
    }

    // Transform argument k into an integer
    int k =  atoi(argv[1]);

    // Check for negative values
    if(k <= 0)
    {
        printf("Key must be a non-negative integer.\n");
        return 1;
    }

    // Get message from user
    string msg = GetString();
    
    // Define length of string
    int l = strlen(msg);

    // Check all values in the string
    for(int i = 0; i < l; i++)
    {

        // Check if value is uppercase letter
        if(isalpha(msg[i]) && isupper(msg[i]))
        {
            //Calculate new position of uppercase letters
            msg[i] = ( ( (msg[i] - UPPER + k) % 26) + UPPER );
        }
    
        // If value is lowecase letter
        if(isalpha(msg[i]) && islower(msg[i]))
        {
            //Calculate new position of lowercase letters
            msg[i] = ( ( (msg[i] - LOWER + k) % 26) + LOWER );
        }
    }
    

    // Print the message
    for (int i = 0; i < l; i++)
    {
        printf("%c", msg[i]);
    }

    // Print a single break
    printf("\n");

return 0;

}