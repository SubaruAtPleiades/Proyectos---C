#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    if (argc != 2 || atoi(argv[1]) < 1) //If the user doesn't input only two arguments or a positive integer, the program will abort and return 1.
    {
        printf("FATAL ERROR! Make sure to pass a maximum of two arguments. 2nd argument must be a positive number\n");
        return 1;
    }

    char *p = get_string("Plaintext: "); //Initializes a string literal with the get_string function. In this case, it will prompt for any text in particular.
    int k = atoi(argv[1]), i = 0; //Stores the integer value of string #2 inside 'k'. Initializes the variable 'i' at 0, so it can work as an indexer.
    printf("Ciphertext: ");
    
    while (p[i] != '\0') //The while loop is used to iterate through the string. The boolean expression "not equal" is to stop the loop when the index reaches the null terminator.
    {
        p[i] += k; //Adds the value of 'k' to the ith-character of the string p.
        if ((p[i] > 90 && p[i] < 97) || p[i] > 122) //Checks if the ith-character has passed both upper and lowercase Z. If so, subtract 26 so it can get the correct value.
        {
            p[i] -= 26;
            printf("%c", p[i]); //Prints the ith-character - 26.
        }
        else
        {
            printf("%c", p[i]); //Prints the ith-character on the screen.
        }
        i++; //Increments the value of the indexer by 1.
    }

    printf("\n"); //Prints a new line.
    return 0; //End.
}