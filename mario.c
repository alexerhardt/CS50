#include <stdio.h>
#include <cs50.h>

int main(void)

{
    int height;
    
    do 
    {
        printf("Give me a non-negative integer no greater than 23: ");
        height = GetInt();
    }
    while (height < 0 || height > 23 );
    
    
    for (int row = 1; row <= height; row++)
    {
        for (int space_count = 1; space_count <= height - row; space_count++)
        {
            printf(" ");
        }
        
        for (int hash_count = 1; hash_count <= row + 1; hash_count++)
        {
            printf("#");
        }
        
        printf("\n");
    }
    
    
    
}