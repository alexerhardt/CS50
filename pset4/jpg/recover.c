/** 

PSEUDOCODE

Include MS common data types

Open infile card.raw
Create an image counter variable
Read next block of memory // create a buffer
    If I'm looking for the first JPEG
        If first four bytes of buffer match JPEG ID // write a function to check this
            Switch a flag to found
            Open an outfile #ID000 // need counter for this, plus way of naming the file according to counter
            Write block to current outfile
            Go to #2
        If first four bytes do not match
            Go to #2 // = don't do anything
    If I'm not looking for the first JPEG
        If first four bytes of buffer match JPEG ID
            Close previous outfile
            Add +1 to the file counter
            Open new outfile
            Write block to current outfile
        If first four bytes of buffer do not match JPEG ID
            Write block to current current outfile
Close remaining files 

**/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "defs.h"

// TODO: TYPEDEF 512 AS MAGIC NUMBER

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
    BYTE* memBlock = malloc ( sizeof(BYTE) * 512 ); 
    
    // TODO MAYBE: Create a pointer to the buffer to check the first four bytes
    
    // Read blocks of memory into buffer TO CHECK - IS THIS SYNTAX OK
    while (fread(memBlock, sizeof(BYTE) * 512, 1, inptr) == 1)
    {
        // If still looking for first JPEG
        if (!found)
        {
            // If first four bytes match JPEG sig, then flag first pic as found, open new file, and write the block
            if (jpgChck(memBlock))
            {
                found = true;
                
                // TODO MAYBE: Looks like all this could be done in a function - DO I NEED TO FREAD AGAIN?
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
                
                // TODO MAYBE: Looks like all this could be done in a function - DO I NEED TO FREAD AGAIN?
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
        
        // TODO: Go to next memory block
        // fseek(inptr, 512, SEEK_CUR);
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