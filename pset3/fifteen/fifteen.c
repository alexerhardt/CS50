/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// playing board
int board[DIM_MAX][DIM_MAX];

// winner "mirror" board
int winner[DIM_MAX][DIM_MAX];

// dimensions
int d;

// position of zero
int zeroRow;
int zeroCol;

// position of user input
int tileRow;
int tileCol;

// prototypes
void clear(void);
void greet(void);
void init(void);
void initwin(void);
void draw(void);
bool move(int tile);
bool won(void);
bool search(int tile);


int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board and set position values of zero
    init();
    initwin();
    zeroRow = d-1;
    zeroCol = d-1;

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);
        
         /* DEBUG ERASE ME WHEN DONE log the current state of winning board
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%3i", winner[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, " |");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);
        */
        
        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // Maximum number according to dimensions of board
    int maxnum = (d * d - 1);
    
    // Populates board starting at maxnum, decreasing numbers by one
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            board[i][j] = maxnum;
            maxnum -= 1;
        }
    }
    
    // If the board has even sides, then switch positions of 1 and 2
    if(d % 2 == 0)
    {
        int temp = board[d-1][d-3];
        board[d-1][d-3] = board[d-1][d-2];
        board[d-1][d-2] = temp;
    }
  
}

/**
 * Initializes a winner "mirror" board to compare the playing board against
 * 
 */
void initwin(void)
{
    // Number initialization
    int initnum = 1;
    
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            winner[i][j] = initnum;
            initnum += 1;
        }
    }
    
    // Override last number with zero
    winner[d-1][d-1] = 0;
}


/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // Prints all present values of the matrix
   for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            
            // If board value is zero, print an underscore
            if(board[i][j] == 0)
            {
                printf(" __");
            }
            
            // In all other cases print the board value
            else
            {
                printf("%3i", board[i][j]);
            }
            
            // Print a pipe for readability
            if (j < d - 1)
            {
                printf(" |");
            }

        }
            printf("\n");
    }
    
    // DEBUG: PRINT VALUES OF ROW / COL
    printf("tileRow: %i\n", tileRow);
    printf("tileCol: %i\n", tileCol);
    printf("zeroRow: %i\n", zeroRow);
    printf("zeroCol: %i\n", zeroCol);
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // Searches tile input by user CAN THIS BE REMOVED?
    search(tile);
    
    // If tile has been found, and is adjacent to zero, swap positions of tile and zero
    // TODO: Refine IF condition if I have time, it's quite messy and can be streamlined)
    if( ( search(tile) ) && ( ( zeroCol == tileCol || zeroRow == tileRow) && 
    ( (tileCol == zeroCol +1) || (tileCol == zeroCol -1) || (tileRow == zeroRow +1) || (tileRow == zeroRow -1) ) ) )
    {
        int temp = board[zeroRow][zeroCol];
        board[zeroRow][zeroCol] = board[tileRow][tileCol];
        board[tileRow][tileCol] = temp;
        zeroRow = tileRow;
        zeroCol = tileCol;
        return true;
    }
    
    return false;
}


/**
 * Search function, returns true if tile is found, false otherwise
 * 
 */
bool search(int tile)
{
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            // If it finds the value, stores tileRow and tileCol positions, and returns true
            if(tile == board[i][j])
            {
                tileRow = i;
                tileCol = j;
                return true;
            }
        }
        
    }
    
    return false;
}



/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    for(int i = 0; i < d; i++)
    {
        for(int j = 0; j < d; j++)
        {
            if(board[i][j] != winner[i][j])
            {
                return false;
            }
        }
    }
    return true;
}


