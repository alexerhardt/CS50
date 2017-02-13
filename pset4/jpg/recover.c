#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "defs.h"

#define BLOCKSIZE 512

bool jpgChck(BYTE *block);

int main(void) {
    
    // Open input file, return error if not found
    FILE* inptr = fopen("card.raw", "r");
    if (inptr == NULL)
    {
        printf("Could not open card.raw\n");
        return 1;
    }
    
    // Create a file counter, a string for the extension, and an array to piece everything together
    // Thanks to: http://stackoverflow.com/questions/16425148/c-fopen-call-with-variable-name
    int fileCount = 0;
    char* extension = ".jpg";
    char fileName[sizeof(int) * 3 + strlen(extension) + 1];
    FILE *outptr;
    
    // Create a flag variable to signal whether first image has been found or not
    bool found = false;
    
    // Allocate memory for block buffer
    BYTE* memBlock = malloc (BLOCKSIZE); 
    
    // Read blocks of memory into buffer TO CHECK - IS THIS SYNTAX OK
    while (fread(memBlock, BLOCKSIZE, 1, inptr) == 1)
    {
        // If still looking for first JPEG
        if (!found)
        {
            // If first four bytes match JPEG sig, then flag first pic as found, open new file, and write the block
            if (jpgChck(memBlock))
            {
                found = true;
                
                // TODO MAYBE: Looks like all this could be done in a function
                // ALSO NEED TO CHECK IF THERE'S SPACE TO CREATE OUTFILE
                snprintf (fileName, sizeof( fileName ), "%03d%s", fileCount, extension);
                outptr = fopen (fileName, "w");
                fwrite (memBlock, sizeof(BYTE) * 512, 1, outptr);
            }
        }
        
        // If it's not the first JPEG
        else
        {
            // If first four bytes of the block match JPEG sig, close previous file, open new one and write the block
            if (jpgChck(memBlock))
            {
                fclose (outptr);
                fileCount += 1;
                
                snprintf (fileName, sizeof( fileName ), "%03d%s", fileCount, extension);
                outptr = fopen (fileName, "w");
                fwrite (memBlock, sizeof(BYTE) * 512, 1, outptr);
            }
            
            // If first four bytes do not match, write block into current file
            else
            {
                fwrite (memBlock, sizeof(BYTE) * 512, 1, outptr);
            }
        }
    }
    
    // Free allocated memory for memory block buffer
    free(memBlock);
    
    // Close input and output file
    fclose(inptr);
    fclose(outptr);
    
    
    return 0;
}

// Checks the first four bytes of a block; returns true if JPG signature is detected, false otherwise
bool jpgChck(BYTE *block)
{
    if(*(block) == 0xff && *(block + 1) == 0xd8 && *(block + 2) == 0xff &&
                ( *(block + 3) >= 0xe0 && *(block +3) <= 0xef ) )
    {
        return true;
    }
    return false;
}
