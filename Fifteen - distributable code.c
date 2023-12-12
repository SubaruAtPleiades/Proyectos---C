// Implements Game of Fifteen (generalized to d x d)

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Constants
#define DIM_MIN 3
#define DIM_MAX 9
#define COLOR "\033[32m"

// Board
int board[DIM_MAX][DIM_MAX];

// Dimensions
int d;

// Saved locations of the blank tile
int blank_row;
int blank_col;

// Prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void swap(int *a, int *b);
void print_grid_row(int d);
void print_tile(int tile);

int main(int argc, string argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // Ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // Open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // Greet user with instructions
    greet();

    // Initialize the board
    init();

    // Accept moves until game is won
    while (true)
    {
        // Clear the screen
        clear();

        // Draw the current state of the board
        draw();

        // Log the current state of the board (for testing)
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

        // Check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // Prompt for move
        printf("Tile to move: ");
        int tile = get_int("");

        // Quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // Log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // Move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // Sleep thread for animation's sake
        usleep(50000);
    }

    // Close log
    fclose(file);

    // Success
    return 0;
}

// Clears screen using ANSI escape sequences
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

// Greets player
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

// Initializes the game's board with tiles numbered 1 through d*d - 1
// (i.e., fills 2D array with values but does not actually print them)
void init(void)
{
    // The outer loop iterates over each column of the board.
    for (int i = 0, first_number_per_column = 0; i < d; i++)
    {
        // For each row, we start with the highest number on the leftmost tile.
        // The highest number is d * d - 1 (since we're counting from 0), and we subtract 'i' to decrease the number for each row.
        board[first_number_per_column][i] = d * d - 1 - i;

        // The inner loop iterates over the rest of the tiles in the current row.
        for (int l = 1; l < d; l++)
        {
            // For each tile, we set its value to be 'd' less than the tile to its left.
            board[l][i] = board[l - 1][i] - d;

            // If we're at the last tile (bottom-right corner), we set its value to 0 to represent the empty space.
            if (i == d - 1 && l == d - 1)
            {
                board[l][i] = 0;
            }
        }
    }

    // If the dimension 'd' is even, we swap the second and third last tiles to ensure the puzzle is solvable.
    if (d % 2 == 0)
    {
        int temp = board[d - 1][d - 2];
        board[d - 1][d - 2] = board[d - 1][d - 3];
        board[d - 1][d - 3] = temp;
    }
}

// Prints the board in its current state
void draw(void)
{
    // Define a minimum and maximum for random number generation, used for color codes.
    int min = 31;
    int max = 33;

    // Seed the random number generator with the current time.
    srand(time(NULL));

    // Iterate over each row of the board.
    for (int i = 0; i < d; i++)
    {
        // Within each row, iterate over each column.
        for (int j = 0; j < d; j++)
        {
            // If the current tile is 0 (the empty space), print an underscore.
            if (board[i][j] == 0)
            {
                printf("  _  ");
            }
            // Otherwise, print the number on the tile.
            else
            {
                // Generate a random number between 'min' and 'max' (for color codes).
                int random_n = (rand() % (max - min + 1)) + min;

                // Print the number on the tile with a random color.
                // "\033[%dm %2i \033[0m" is a format string that includes an ANSI escape code for color.
                printf("\033[%dm %2i \033[0m", random_n, board[i][j]);
                printf(" ");
            }
        }
        // Print a newline at the end of each row.
        printf("\n");
    }

    // Print an additional newline at the end for spacing.
    printf("\n");
}

// If tile borders empty space, moves tile and returns true, else returns false
bool move(int tile)
{

    // Declares three int type variables. Two to hold the index of both the row and column when the tile number is found
    // and the last one to serve as a flag.
    int index1, index2, invalid;


    // Both the outer and inner loops serve to find the right tile through the whole two dimensional array.
    for (int a = 0; a < d; a++)
    {

        for (int b = 0; b < d; b++)
        {
            // If the tile is found, then we save the row and column index for later.
            if (board[a][b] == tile)
            {
                index1 = a, index2 = b;

                // This is the movement logic. For example, in this first check, we search if the empty space is
                // located at the right side of the tile. If so, swap them. The respective checks needed
                // to ensure we don't go beyond the array boundaries are also included.
                if (board[a][b + 1] == 0 && b != d - 1)
                {
                    board[a][b + 1] = tile;
                    board[index1][index2] = 0;
                }

                // Checks if the blank space is at the left.
                else if (board[a][b - 1] == 0 && b != d - d)
                {
                    board[a][b - 1] = tile;
                    board[index1][index2] = 0;
                }

                // Checks if the blank space is below the tile.
                else if (board[a + 1][b] == 0 && a != d - 1)
                {
                    board[a + 1][b] = tile;
                    board[index1][index2] = 0;
                }

                // Checks if the blank space is above the tile
                else if (board[a - 1][b] == 0 && a != d - d)
                {
                    board[a - 1][b] = tile;
                    board[index1][index2] = 0;
                }

                // If the blank space is not found either above, down, at the left or at the right of the tile,
                // we assume an invalid movement was attempted, so we activate the invalid flag and break the inner loop.
                else
                {
                    invalid = 1;
                    break;
                }

                // If a valid move was done, we return true.
                return true;
            }
        }

        // Checks if the flag is activated, if so, break the loop and return false.
        if (invalid == 1)
        {
            break;
        }
    }

    return false;

}

// Returns true if game is won (i.e., board is in winning configuration), else false
bool won(void)
{
    // We start with a counter at 1, which represents the number we expect to see on the first tile.
    int counter = 1;

    // Flag variable to check if there's a tile out of place.
    int wrong_configuration;

    // We iterate through each row of the board.
    for (int w = 0; w < d; w++)
    {
        // For each row, we iterate through each column.
        for (int i_w = 0; i_w < d; i_w++)
        {
            // If the current tile matches the counter, we increment the counter.
            if (board[i_w][w] == counter)
            {
                counter++;

                // If the counter equals the total number of tiles (d * d), we've checked every tile
                // and they were all in order, so the game is won.
                if (counter == d * d)
                {
                    return true;
                }
            }
            // If the current tile doesn't match the counter and we're not at the last tile (the empty space),
            // the board isn't in the correct order, so the game isn't won.
            else
            {
                wrong_configuration = 1;
                break;
            }
        }

        // If a tile was out of place, we break the outer loop as well.
        if (wrong_configuration == 1)
        {
            break;
        }
    }

    // If we've gone through the entire board and didn't return true, then the game isn't won.
    return false;
}
