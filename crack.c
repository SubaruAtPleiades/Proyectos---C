/*Hello, professors of Edutin Academy! Here Benjamin, and after a long week I can finally show you the finished "crack" program.
*
*It was easily the hardest activity on this course, but I enjoyed it a lot!
*A first but very redundant version of this project was made using inner for loops, but I was not
*satisfied with the design of it, even if it worked as it should. So after a few more days of burning
*my brain, I come out with this version. It uses three macros to avoid unnecessary "magic numbers", but also
*two functions that I created to make the program work. Check the comments for both functions to learn what
*they do!
*
*To compile, it's recommended to use (without quotes): "clang -O3 -o crack crack.c -lcrypt" to get the best
*performance.
*As final note, remember that the time this program takes to crack the password, will depends at the time
*you are using the cloud IDE. Desktop Visual Studio Code, should get much lower times even with 5-char passwords.
*
*Enjoy and happy coding!*/

#include <stdio.h>
#include <crypt.h>
#include <string.h>
#define FIRST_LETTER 0             //Macro used for the first letter of the crack string. See the comments below for more information.
#define PASSWORD_SALT "17"        //Macro used for the Salt. Take in mind that even if I used the string "17", the string literal can be changed to your liking.
#define REMAINING_ELEMENTS 4     //Macro used inside the crk function. See the comments of the function for more information.
#define ALPHA_SIZE 52           //Macro used for determining the size of the english alphabet, including both lower and uppercase letters.

/*Function declarations for the crk and jmplu functions.*/
void next_letter_index(char next_letter[], int *A1, int *L1, int next_counter[]);
void jump_to_lower_or_upper(char lower_upper[], int *L2);

int main(int argc, char *argv[])
{
    /*Two variables and one array of type int, used mainly in the next_letter_index and jump_to_lower_or_upper functions.*/
    int attempt = 0, counter[4], length = 1;

    /*A char array. It's used to crack the password. Null terminators are used to control the length of the string.*/
    char crack[] = {'A', '\0', '\0', '\0', '\0', '\0'};

    /*A conditional branch that checks if you don't input only two arguments, returning 1 if so.*/
    if (argc != 2)
    {
        fprintf(stderr, "Fatal error! Only one argument(hash) aside from %s is allowed...\n", argv[0]);
        return 1;
    }

   /*For context, any element of the counter array is used to count the number of times a letter has iterated,
    *from 'A' to 'z' which is a total of 52 times. The first counter at index 0, has the job to
    *check the number of iterations of the first letter of the string. We initialize it at 1, since
    *we will be checking the first letter in ASCII which is the uppercase 'A' during the first execution.
    *
    *The while loop is where things get interesting. It will loop forever, checking with the
    *strcmp function if the hash input by the user matches with the hashed password of the string crack.
    *When a match is found, the loop will break. This loop has no room for failure as it will eventually find a match.
    *Take in mind, that this program only can crack passwords made of alphabetic characters up to five chars.
    *If you input a hash that was not crypted as alpha chars the loop will never find a match.*/

    counter[0] = 1;

    while (1)
    {
        if (strcmp(argv[1], crypt(crack, PASSWORD_SALT)))
        {
            crack[FIRST_LETTER]++; //Increments the ASCII value of the first letter of the string.
            attempt++; //Increments the number of attempts made to crack the password.
            counter[0]++; //Increments the value of the first counter.
            next_letter_index(crack, &attempt, &length, counter); //Calls function next_letter_index, passing the string "crack" as argument, along with the address of lenght and attempt.
            jump_to_lower_or_upper(crack, &length); //Calls function jump_to_lower_or_upper, passing the string "crack" and the address of "length" as arguments.
        }
        else
        {
            break;
        }

    }

    /*After a match is found, we have these branches. The first is only used if we crack the password on the first try, which of course
     would be pretty uncommon. The second is used the majority of the time. Both of them tell the user that the crack has succefully
     cracked the password, printing the attempts made to crack it and the name of the password*/
    if (attempt == 0)
    {
        printf("Success! The password is: %s\n", crack);
    }
    else
    {
        printf("Success after attempt %i! The password is: %s\n", attempt, crack);
    }
    return 0;

}

/*This is the definition of the next_letter_index function
 *Its return type is void, and only takes one argument as a string or an array of chars*/
void next_letter_index(char next_letter[], int *A1, int *L1, int next_counter[])
{
    int next_index = 0; //Variable that works as temporal index for the string.
    for (int c = 0, next_position = 1; c < REMAINING_ELEMENTS; c++, next_position++) //A loop that iterates through the remaining elements of the counter array. REMAINING ELEMENTS is used to avoid magic numbers.
    {
        if (next_counter[c] > ALPHA_SIZE) //Checks if the counter at index 'c' is greater than 52.
        {
            next_index = next_position;   //next_index is initialized with the value of 'next_position', which will have the value of the next letter of the string.
            c++;             //Increments the value of 'c'. We do this to temporarily increment the value of the next counter as you see in the next line.
            next_counter[c]++;
            c--;             //Restores the value of 'c' to the one used in the current iteration.
            next_counter[c] = 1;  //Restores the value of the counter to 1. In doing this, we can check again when we need to increment the value of the next letter again after 52 iterations.

            /*After modifying the index of the string to the right value, we use this branches to check either of the next things:
             1. If the element 'z' of the string not equals to NULL terminator, increment the value of that letter.
             2. If the element 'z' of the string equals to NUll terminator, assing it the value of the uppercase A, increment the
                value of the length variable, and reset the number of attempts at 0.*/
            if (next_letter[next_index] != '\0')
            {
                next_letter[next_index]++;
            }
            else if (next_letter[next_index] == '\0')
            {
                next_letter[next_index] = 'A';
                (*L1)++;
                *A1 = 0;
            }
        }
    }

}

/*Definition of the jump_to_lower_or_upper function.
 *Just as next_letter_index, its return type is void. It only has 2 parameters, an array of chars and an integer.*/
void jump_to_lower_or_upper(char lower_upper[], int *L2)
{
    for (int l = 0; l < *L2; l++) //A loop that iterates through every letter of the string. The number of times it does so is linked to the length of the string passed as an argument.
    {
        /*A very simple conditional branch that checks either if the element 'l' of the string is greater than the 'Z' or 'z'
          and if so, then resets its value to either 'A' if its greater than 'z' and 'a' if it's greater than 'Z'*/
        if (lower_upper[l] == 'Z' + 1)
        {
            lower_upper[l] = 'a';
        }
        else if (lower_upper[l] == 'z' + 1)
        {
            lower_upper[l] = 'A';
        }y
    }
}