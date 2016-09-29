#include <stdio.h>
#include <cs50.h>
#include <string.h>

int main (void)
{
    // The name the user inputs
    string name;
    
    // The length of the name string
    int length;

    // Ask user for name, store in string and define string length
    // printf("Give me your name please: ");
    name = GetString();
    length = strlen(name);

    // Check all characters in the string
    for(int i = 0; i <= length; i++)
    {
        // If the characer is lowercase, switch to uppercase
        if (name[i] >= 'a' && name[i] <= 'z')
        name[i] -= 'a' - 'A';
    }
    
    // Print first letter of the word
    printf("%c", name[0]);

    // Check characters of the array for spaces
    for(int i = 0; i <= length; i++)
    {
        // If a space is found, print the char immediately after it
        if(name[i] == ' ')
        printf("%c", name[i+1]);
    }
    
    // Print a line break at the end
    printf("\n");
    
}