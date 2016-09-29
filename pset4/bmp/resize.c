/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Resizes a BMP by a factor of n
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"


int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // check for validity of n
    int n = atoi(argv[1]);
    if(n < 1 || n > 100)
    {
        printf("Please enter a positive integer between 1 and 100");
        return 2;
    }
    
    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) fa 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }
    
    // determine old padding (to be used later)
    int inPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // change image width and height in header
    BITMAPINFOHEADER old;
    old.biWidth = bi.biWidth;
    old.biHeight = bi.biHeight;
    bi.biWidth = n * bi.biWidth;
    bi.biHeight = n * bi.biHeight;
    
    // determine new padding
    int newPadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // determine new image size
    bi.biSizeImage = abs(bi.biHeight) * (sizeof(RGBTRIPLE) * bi.biWidth + newPadding);

    // change filesize in header
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // allocate memory for array that will memorize the new pixel line
    RGBTRIPLE *newLine = malloc(sizeof(RGBTRIPLE) * bi.biWidth);

    // iterate over infile's scanlines
    for (int lineCount = 0, biHeight = abs(old.biHeight); lineCount < biHeight; lineCount++)
    {
        // set array pointer to beginning of temporary pixel storage
        RGBTRIPLE *arrayPtr = newLine;
        
        // iterate over pixels in scanline
        for (int pixCount = 0; pixCount < old.biWidth; pixCount++)
        {
            // temporary storage
            RGBTRIPLE triple;
    
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
            // write RGB triple n times into temp array
            for (int tripleCount = 0; tripleCount < n; tripleCount++) 
            {
                *arrayPtr = triple;
                arrayPtr++;
            }
        }
        
        // create new line of pixels and padding n times in outfile
        for (int addLine = 0; addLine < n; addLine ++)
        {
            fwrite(newLine, sizeof(RGBTRIPLE) * bi.biWidth, 1, outptr);
                
                // add appropriate amount of padding
                for (int padCount = 0; padCount < newPadding; padCount++)
                {
                    fputc(0x00, outptr);
                }
        }
        
        // skip over padding in input file
        fseek(inptr, inPadding, SEEK_CUR);
    }

    // free the memory allocated
    free(newLine);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
