// Implements Scramble in C, where # is an optional grid number.

#include <cs50.h>
#include <ctype.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Duration of a game in seconds
#define DURATION 30

// Grid's dimensions
#define DIMENSION 4

// Maximum number of words in any dictionary
#define WORDS 172806

// Maximum number of letters in any word
#define LETTERS 29

// Default dictionary
// http://www.becomeawordgameexpert.com/wordlists.htm
#define DICTIONARY "words.txt"

// For logging
FILE *logfile;

// Grid
char grid[DIMENSION][DIMENSION];

// Flags with which we can mark grid's letters while searching for words
bool marks[DIMENSION][DIMENSION];

// Defines a word as having an array of letters plus a flag
// Indicating whether word has been found on grid
typedef struct
{
    bool found;
    char letters[LETTERS + 1];
}
word;

// Defines a dictionary as having a size and an array of words
struct
{
    int size;
    word words[WORDS];
}
dictionary;

// Prototypes
void clear(void);
bool crawl(string letters, int x, int y);
void draw(void);
bool find(string s);
void initialize(void);
bool load(string s);
bool lookup(string s);
void scramble(void);


int main(int argc, string argv[])
{
    // Ensure proper usage
    if (argc > 2)
    {
        printf("Usage: %s [#]\n", basename(argv[0]));
        return 1;
    }

    // Seed pseudorandom number generator
    if (argc == 2)
    {
        int seed = atoi(argv[1]);
        if (seed <= 0)
        {
            printf("Invalid grid.\n");
            return 1;
        }
        srand(seed);
    }
    else
    {
        srand(time(NULL));
    }

    // Determine path to dictionary
    string directory = dirname(argv[0]);
    char path[strlen(directory) + 1 + strlen(DICTIONARY) + 1];
    sprintf(path, "%s/%s", directory, DICTIONARY);

    // Load dictionary
    if (!load(path))
    {
        printf("Could not open dictionary.\n");
        return 1;
    }

    // Initialize the grid
    initialize();

    // Initialize user's score
    int score = 0;

    // Calculate time of game's end
    int end = time(NULL) + DURATION;

    // Open log
    logfile = fopen("./log.txt", "w");
    if (logfile == NULL)
    {
        printf("Could not open log.\n");
        return 1;
    }

    // Accept words until timer expires
    while (true)
    {
        // Clear the screen
        clear();

        // Draw the current state of the grid
        draw();

        // Logfile board
        for (int row = 0; row < DIMENSION; row++)
        {
            for (int col = 0; col < DIMENSION; col++)
            {
                fprintf(logfile, "%c", grid[row][col]);
            }
            fprintf(logfile, "\n");
        }

        // Get current time
        int now = time(NULL);

        // Report score
        printf("Score: %d\n", score);
        fprintf(logfile, "%d\n", score);

        // Check for game's end
        if (now >= end)
        {
            printf("\033[31m"); // red
            printf("Time:  %d\n\n", 0);
            printf("\033[39m"); // default
            break;
        }

        // Report time remaining
        printf("Time:  %d\n\n", end - now);

        // Prompt for word
        printf("> ");
        string s = get_string(" ");

        // Quit playing if user hits ctrl-d
        if (s == NULL)
        {
            break;
        }

        // Capitalize word
        for (int i = 0, n = strlen(s); i < n; i++)
        {
            s[i] = toupper(s[i]);
        }

        // Logfile word
        fprintf(logfile, "%s\n", s);

        // Check whether to scramble grid
        if (strcmp(s, "SCRAMBLE") == 0)
        {
            scramble();
        }

        // Or to look for word on grid and in dictionary
        else
        {
            if (find(s) && lookup(s))
            {
                score += strlen(s);
            }
        }
    }

    // Close log
    fclose(logfile);

    return 0;
}

// Clears screen
void clear()
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

// Returns true iff all letters are found
bool crawl(string letters, int x, int y)
{
    // If out of letters, then we must've found them all!
    if (strlen(letters) == 0)
    {
        return true;
    }

    // Don't fall off the grid!
    if (x < 0 || x >= DIMENSION)
    {
        return false;
    }
    if (y < 0 || y >= DIMENSION)
    {
        return false;
    }

    // Been here before!
    if (marks[x][y])
    {
        return false;
    }

    // Check grid[x][y] for current letter
    if (grid[x][y] != letters[0])
    {
        return false;
    }

    // Mark location
    marks[x][y] = true;

    // Look left and right for next letter
    for (int i = -1; i <= 1; i++)
    {
        // Look down and up for next letter
        for (int j = -1; j <= 1; j++)
        {
            // Check grid[x + i][y + j] for next letter
            if (crawl(&letters[1], x + i, y + j))
            {
                return true;
            }
        }
    }

    // Unmark location
    marks[x][y] = false;

    // Fail
    return false;
}

//Prints the grid in its current state
void draw(void)
{
    // Define the range of colors to use (31 to 37 are ANSI color codes for red to white)
    int min = 31, max = 37;

    // Iterate over each row in the grid
    for (int grid_rows = 0; grid_rows < DIMENSION; grid_rows++)
    {
        // Print a dividing line at the start of each row for better visual clarity
        printf("———————————————\n");

        // Iterate over each column in the current row
        for (int grid_columns = 0; grid_columns < DIMENSION; grid_columns++)
        {
            // Generate a random color within the defined range
            int random_number = (rand() % (max - min + 1)) + min;

            // Print the grid character with the selected color, then reset color
            printf("\033[%dm%c\033[0m", random_number, grid[grid_rows][grid_columns]);

            // Print a divider between each character for better visual clarity
            printf(" | ");
        }

        // Print a newline at the end of each row
        printf("\n");
    }

    // Print a final dividing line at the end of the grid
    printf("———————————————\n");
}

// Returns true iff word, s, is found in grid
bool find(string s)
{
    // Word must be at least 2 characters in length
    if (strlen(s) < 2)
    {
        return false;
    }

    // Search grid for word
    for (int row = 0; row < DIMENSION; row++)
    {
        for (int col = 0; col < DIMENSION; col++)
        {
            // Reset marks
            for (int i = 0; i < DIMENSION; i++)
            {
                for (int j = 0; j < DIMENSION; j++)
                {
                    marks[i][j] = false;
                }
            }

            // Search for word starting at grid[i][j]
            if (crawl(s, row, col))
            {
                return true;
            }
        }
    }
    return false;
}

// Initializes grid with letters
void initialize(void)
{
    // http://en.wikipedia.org/wiki/Letter_frequency
    float frequencies[] = {
        8.167,  // a
        1.492,  // b
        2.782,  // c
        4.253,  // d
        12.702, // e
        2.228,  // f
        2.015,  // g
        6.094,  // h
        6.966,  // i
        0.153,  // j
        0.747,  // k
        4.025,  // l
        2.406,  // m
        6.749,  // n
        7.507,  // o
        1.929,  // p
        0.095,  // q
        5.987,  // r
        6.327,  // s
        9.056,  // t
        2.758,  // u
        1.037,  // v
        2.365,  // w
        0.150,  // x
        1.974,  // y
        0.074   // z
    };
    int n = sizeof(frequencies) / sizeof(float);

    // Iterate over grid
    for (int row = 0; row < DIMENSION; row++)
    {
        for (int col = 0; col < DIMENSION; col++)
        {
            // Generate pseudorandom double in [0, 1]
            double d = rand() / (double) RAND_MAX;

            // Map d onto range of frequencies
            for (int k = 0; k < n; k++)
            {
                d -= frequencies[k] / 100;
                if (d < 0.0 || k == n - 1)
                {
                    grid[row][col] = 'A' + k;
                    break;
                }
            }
        }
    }
}

// Loads words from dictionary with given filename, s, into a global array.
bool load(string s)
{
    // Open dictionary
    FILE *file = fopen(s, "r");
    if (file == NULL)
    {
        return false;
    }

    // Initialize dictionary's size
    dictionary.size = 0;

    // Load words from dictionary
    char buffer[LETTERS + 2];
    while (fgets(buffer, LETTERS + 2, file))
    {
        // Overwrite \n with \0
        buffer[strlen(buffer) - 1] = '\0';

        // Capitalize word
        for (int i = 0, n = strlen(buffer); i < n; i++)
        {
            buffer[i] = toupper(buffer[i]);
        }

        // Ignore SCRAMBLE
        if (strcmp(buffer, "SCRAMBLE") == 0)
        {
            continue;
        }

        // Copy word into dictionary
        dictionary.words[dictionary.size].found = false;
        strncpy(dictionary.words[dictionary.size].letters, buffer, LETTERS + 1);
        dictionary.size++;
    }

    // Success!
    return true;
}

// Looks up word, s, in dictionary.  If found (for the first time), flags word as found (so that user can't score with it again) and returns true
bool lookup(string s)
{
    // Declare necessary variables for binary search.
    int size, low, mid, high, target;

    // Set the size of the global array.
    size = WORDS;
    // Set the lower boundary of the search space to the start of the string.
    low = size - size;  // This is essentially equivalent to low = 0 haha!;
    // Set the upper boundary of the search space to the end of the string.
    high = size - 1;

    // Continue the search as long as the lower boundary is less than or equal to the upper boundary.
    while (low <= high)
    {
        // Calculate the middle point of the current search space.
        mid = low + (high - low) / 2;

        // Compare the string to the word at the midpoint in the dictionary.
        target = strcmp(s, dictionary.words[mid].letters);

        // If the strings match and the word hasn't been found before:
        if (target == 0 && !dictionary.words[mid].found)
        {
            // Mark the word as found and return true.
            dictionary.words[mid].found = true;
            return true;
        }
        // If the string is lexicographically smaller than the word at the midpoint,
        // adjust the upper boundary to search the lower half of the current search space.
        else if (target < 0)
        {
            high = mid - 1;
        }
        // If the string is lexicographically larger than the word at the midpoint,
        // adjust the lower boundary to search the upper half of the current search space.
        else
        {
            low = mid + 1;
        }
    }

    // If the string could not be found in the dictionary, return false.
    return false;
}

// Scrambles the grid by rotating it 90 degrees clockwise, whereby grid[0][0] rotates to grid[0][DIMENSION - 1]

void scramble(void)
{
    // Create a temporary 2D array to hold the new scrambled values
    char scrambled_grid[DIMENSION][DIMENSION];

    // Iterate over each cell in the original grid
    for (int i = 0; i < DIMENSION; i++)
    {
        for (int j = 0; j < DIMENSION; j++)
        {
            // For each cell, calculate its new position in the scrambled grid
            // rotating it 90 degrees clockwise
            scrambled_grid[j][DIMENSION - 1 - i] = grid[i][j];
        }
    }

    // Now that all the values have been moved to their new positions in the
    // scrambled grid, copy these values back into the original grid
    for (int row = 0; row < DIMENSION; row++)
    {
        for (int column = 0; column < DIMENSION; column++)
        {
            grid[row][column] = scrambled_grid[row][column];
        }
    }
}