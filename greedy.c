#include<stdio.h>
#include<math.h>
#include<cs50.h>

int main(void)

{
    float user_input;
    
    printf("Type amount in dollars: ");
    user_input = GetFloat();
    
    while (user_input < 0)
    {
        printf("Please type a non-negative amount: ");
        user_input = GetFloat();
    }
    
    int round_cents = round(user_input * 100);
    
    /* printf("Round cents is: %i\n", round_cents); */
    
    int total_coins = round_cents / 25;
    int remainder = round_cents % 25;
    
    if (remainder > 0)
    {
        total_coins = total_coins + remainder / 10;
        remainder = remainder % 10;
        
        if (remainder > 0)
        {
            total_coins = total_coins + remainder / 5;
            remainder = remainder % 5;
            
            if (remainder >0)
            {
                total_coins = total_coins + remainder / 1;
                printf("%i\n", total_coins);
            }
            
            else {
                printf("%i\n", total_coins);
            }
            
        }
        
        else
        {
            printf("%i\n", total_coins);
        }
        
        
    }
        
    else
    {
        printf("%i\n", total_coins);
    }
        
        
    }
    
    
    
    