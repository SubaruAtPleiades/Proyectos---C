// Helper functions

#include <cs50.h>
#include "helpers.h"

// Returns true if value is in array of n values, else false
// For this search algorithm of O(log n), we'll use binary search.
bool search(int value, int values[], int n)
{
    // Define boundaries for the search
    int low = n - n; // n - n is the formula for the starting point of the lowest index, as the size of the array - the size of the array equals 0 (although this is of course, self explanatory, haha!)
    int high = n - 1; // For the highest part n - 1, since C arrays start with index 0.

    // Continue searching while there are still potential matches
    while (low <= high)
    {
        // Calculate the midpoint for roughly equal partition
        int mid = low + (high - low) / 2;

        // If the value is found at midpoint
        if (values[mid] == value)
        {
            return true;
        }          
        // If the value is in the left half
        else if (values[mid] > value)
        {
            high = mid - 1;
        }
        // If the value is in the right half
        else
        {
            low = mid + 1;
        }
    }

    // Value wasn't found
    return false;
}

// Sorts array of n values using selection sort.
void sort(int values[], int n)
{
    // Traverse through all array elements
    for (int i = 0; i < n - 1; i++)
    {
        // Find the index of the smallest value in the unsorted part of the array
        int lowest_index = i;

        for (int j = i + 1; j < n; j++)
        {
            // If a smaller value is found, update the index of the smallest value
            if (values[j] < values[lowest_index])
            {
                lowest_index = j;
            }
        }

        // Swap the smallest value found with the first value of the unsorted part
        int temp = values[lowest_index];
        values[lowest_index] = values[i];
        values[i] = temp;
    }
}