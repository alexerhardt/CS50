#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Number of minutes in shower: ");
    int minutes = GetInt();
    
    int bottles = minutes * 12;
    printf("The number of bottles you used is: %d\n", bottles);
    
}