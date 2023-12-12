// Generates pseudorandom numbers in [0,LIMIT), one per line

#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constant
#define LIMIT 65536

int main(int argc, string argv[])
{
    // Conditional check to validate if the user hasn't input either two, or three arguments. If so, then return 1.
    if (argc != 2 && argc != 3)
    {
        printf("Usage: generate n [s]\n");
        return 1;
    }

    // Stores in | n | the second argument as an int using atoi. Remember to use stdlib.h to use this function. The second argument works to set how many pseudorandom numbers will be generated.
    int n = atoi(argv[1]);

    // Checks if the user pass the seed number as the third argument. If so, uses atoi to turn the string into an int, then typcast it into a long int to generate the seed with srand48
    // Otherwise, the program uses the time function (included in time.h) to generate a seed number based on the date and hour.
    if (argc == 3)
    {
        srand48((long int) atoi(argv[2]));
    }
    else
    {
        srand48((long int) time(NULL));
    }

    // Prints all the pseudorandom numbers using a for loop. It uses the drand48 function to generate them multiplied per the constant LIMIT, so it can generate any value between that range. Then typecast the doble into an int.
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // Success
    return 0;
}
