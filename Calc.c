// Include necessary header files
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>
#include "getter.h"


// Define constants
#define ARGUMENT_COUNT 2  // Number of expected command line arguments
#define RETURN_ERROR_CODE 1 // Return code for argument error
#define COMMAND_ARGUMENT_SYNTAX_ERROR 2 // Return code for argument syntax error
#define OPEN "Launch" // Expected second argument
#define MAX_OPERANDS 15 // Max operands in expression
#define MAX_OPERATORS 15 // Max operators in expression
#define MINIMUM_INDEX_FOR_EQUAL_OPERATOR 1 // Minimum index to allow = operator
#define MOD '%' // Modulo operator
#define RESET 1
#define CONTINUE 2

// Function prototypes
void calculator(void);
void draw(long double n[], char o[], int size);
void pad(void);

// Main function
int main(int argc, char *argv[])
{
  // Check argument count
  if (argc != ARGUMENT_COUNT)
  {
    fprintf(stderr, "\nError! Argument count must be %d\n\n", ARGUMENT_COUNT);
    return RETURN_ERROR_CODE;
  }

  // Check second argument
  if (strcmp(argv[1], OPEN))
  {
    fprintf(stderr, "\nError! Usage must be like this: %s %s\nMake sure to use quotations between the second argument\n\n", argv[0], OPEN);
    return COMMAND_ARGUMENT_SYNTAX_ERROR;
  }

  printf("\n");

  // Prompts the user for a name

  printf("\033[H\033[J");
  printf("Hello, please type your name: ");
  char *username = get_string();

  //Prints the name on screen
  printf("\033[H\033[J");
  printf("Welcome, %s! Launching calculator...\n", username);

  sleep(5);

  // Call calculator
  calculator();

  return 0;
}

// Calculator function
void calculator(void)
{
  printf("\033[H\033[J");

  printf("Glossary:\n\n");
  printf("+ to add\n- to subtract\n* to multiply\n/ to divide\n%c to get the rest of a division\nP for powers\n\n", MOD);

  printf("Use the equal operator '=' to get the result. To exit the program use ctrl + c");
  fflush(stdout);

  sleep(5);

  // Structure to hold calculation data
  typedef struct
  {
    long double operands[MAX_OPERANDS];
    char operators[MAX_OPERATORS];
  } Math;

  // Create Math struct
  Math math;

  int string_flag = 0;

  // Initialize flags
  bool int_flag = false, float_flag = false;

  // Updated result variable to  double
  long double result = 0.0;

  // Loop until exit
  do
  {
    int overflow = 0;

    // Initialize struct arrays
    for (int initialize = 0; initialize < MAX_OPERANDS; initialize++)
    {
      math.operands[initialize] = 0.0;
      math.operators[initialize] = 0;
    }

    // Variables to track indices and results
    char *off_or_reset = NULL;
    int n_index = 0, o_index = 0;

    // Clear screen
    printf("\033[H\033[J");
    printf("\n");
    pad();

    // Get input
    while (true)
    {
      int flag = 0;

      // Check if previous operator was power
      if (o_index > 0 && math.operators[o_index - 1] == 'P')
      {
        printf("\nEXPONENT: ");
      }

      // Get operand
      if (string_flag == CONTINUE)
      {
        math.operands[n_index] = result;
        string_flag = 0;
      }
      else
      {
        printf("\n");
        math.operands[n_index] = get_float();
      }

      // Clear screen and draw current expression
      printf("\033[H\033[J");
      pad();
      printf("\n");
      draw(math.operands, math.operators, o_index);
      fflush(stdout);

      // Get operator
      printf("\n");
      do
      {
        math.operators[o_index] = get_char();

        // Evaluates if any valid operator has been stored on the array. This helps to avoid inputing any invalid operator and drawing it on screen.
        switch (math.operators[o_index])
        {
          case '+':
          case '-':
          case '*':
          case '/':
          case '%':
          case 'p':
          case '=':
          flag = 1;

          // Convert power operator to uppercase
          if (math.operators[o_index] == 'p')
          {
            math.operators[o_index] = toupper(math.operators[o_index]);
          }

          // Clear screen and draw current expression
          printf("\033[H\033[J");
          pad();
          printf("\n");
          draw(math.operands, math.operators, o_index);
          fflush(stdout);
          break;

          // If an invalid char has been inputted, then clear the screen.
          default:
          printf("Invalid OPERATOR!\n");
          printf("\033[H\033[J");
          sleep(3);
          math.operators[o_index] = 0;
          pad();
          printf("\n");
          draw(math.operands, math.operators, o_index);
          printf("\n");
          break;
        }
      }
      while (flag != 1);

      // Check for end of expression
      if (math.operators[o_index] == '=' && o_index >= MINIMUM_INDEX_FOR_EQUAL_OPERATOR)
      {
        break;
      }
      // Check for invalid = operator
      else if (math.operators[o_index] == '=' && o_index < MINIMUM_INDEX_FOR_EQUAL_OPERATOR)
      {
        printf("\nError: The equal operator '%c' must not be the first operator in the arithmetic operation.\nPlease enter a valid operator.\n\n", math.operators[o_index]);
        continue;
      }
      else
      {
        // Increment indexes
        ++n_index;
        ++o_index;
      }
    }

    // Set initial result
    result = math.operands[0];

    // Evaluate expression
    for (int i = 0, j = 1; i < o_index; i++, j++)
    {
      switch (math.operators[i])
      {
        // Multiplication
        case '*':
          // Convert to int if float
          if (float_flag == true)
          {
            float_flag = false;
          }

          result *= math.operands[j];
          int_flag = true;
          break;

        // Addition
        case '+':
          // Convert to int if float
          if (float_flag == true)
          {
            float_flag = false;
          }

          result += math.operands[j];
          int_flag = true;
          break;

        // Subtraction
        case '-':
          // Convert to int if float
          if (float_flag == true)
          {
            float_flag = false;
          }

          if (math.operators[i + 1] == 'P')
          {
            math.operands[j] = powl(math.operands[j], math.operands[j + 1]);
            ++i;
          }

          result -= math.operands[j];
          int_flag = true;
          break;

        // Division.
        case '/':

          // Convert to float if int
          if (int_flag == true)
          {
            int_flag = false;
          }

          result /= math.operands[j];
          float_flag = true;
          break;

        // Power
        case 'P':
          // Convert operand to a double precision float
          if (float_flag == true)
          {
            float_flag = false;
          }

          result = powl(result, math.operands[j]);
          int_flag = true;
          break;

        // MOD
        case '%':
          // Convert to int if float
          if (float_flag == true)
          {
            float_flag = false;
          }

          result = fmod(result, math.operands[j]);
          int_flag = true;
          break;
      }

      if ((long long int) result == LLONG_MIN)
      {
        overflow = 1;
        break;
      }

      // Print result if the last operation was not a division.
      if (int_flag == true && math.operators[i + 1] == '=')
      {
        printf("\033[32m%lld\033[0m", (long long int) result);
        break;
      }
      // Print result if the last operation was a division.
      else if (float_flag == true && math.operators[i + 1] == '=')
      {
        printf("%.2Lf", result);
        break;
      }

    }

    if (overflow == 1)
    {
      printf("\nInteger overflow! Try again\n");
      sleep(2);
      string_flag = RESET;
      continue;
    }

    // Get input to clear or exit
    printf("\n\n");
    printf("C or off?: ");
    off_or_reset = get_string();

    // Check input
    if (strcmp(off_or_reset, "C") == 0)
    {
      string_flag = RESET;
      result = 0.0;
    }
    else if (strcmp(off_or_reset, "off") == 0)
    {
      printf("\n");
      printf("\033[H\033[J");
      exit(EXIT_SUCCESS);
    }
    else
    {
      string_flag = CONTINUE;
    }

  }
  while (string_flag == RESET || string_flag == CONTINUE);

}

// Function to draw current expression
void draw(long double n[], char o[], int size)
{
  for (int i = 0; i <= size; i++)
  {
    // Draw operands
    if (n[i] != 0)
    {
      printf("\033[1m\033[32m%.2Lf  \033[0m", n[i]);
    }


    // Draw operators
    if (o[i] != 0)
    {
      if (o[i] == 'P')
      {
        printf("\033[1m\033[36m%c  \033[0m", '^');
      }
      else if(o[i] == '*' || o[i] == '+' || o[i] == '-' || o[i] == '/' || o[i] == '%' || o[i] == '=')
      {
        printf("\033[1m\033[36m%c  \033[0m", o[i]);
      }

    }

  }
}

void pad(void)
{
    printf("\033[33m _______________________\033[0m\n");
    printf("\033[33m| 7   |  8  |  9  |  +  |\033[0m\n");
    printf("\033[33m|_____|_____|_____|_____| \033[0m\n");
    printf("\033[33m|  4  |  5  |  6  |  -  |\033[0m\n");
    printf("\033[33m|_____|_____|_____|_____| \033[0m\n");
    printf("\033[33m|  1  |  2  |  3  |  *  |\033[0m\n");
    printf("\033[33m|_____|_____|_____|_____| \033[0m\n");
    printf("\033[33m|  0  |  .  |  =  |  /  |\033[0m\n");
    printf("\033[33m|_____|_____|_____|_____| \033[0m\n");
    printf("\033[33m|  P  |  %c  |\033[0m\n", '%');
    printf("\033[33m|_____|_____| \033[0m\n");
}