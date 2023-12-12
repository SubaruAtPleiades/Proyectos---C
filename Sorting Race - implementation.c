// Helper functions for the sort race

#include <cs50.h>
#include "helpers.h"

// Returns true if str is a valid flag (-a, -b, -r, or -s), false otherwise
bool check_flag(char *str)
{
    // Check if the first character is a hyphen and the second character is a valid flag
    if (str[0] != '-' || (str[1] != 'a' && str[1] != 'b' && str[1] != 'r' && str[1] != 's'))
    {
        return false; // Invalid flag
    }
    else
    {
        return true; // Valid flag
    }
}

// Sorts valuesay of n values using bubble sort
void bubble(int values[], int n)
{
    bool swapped; // A flag to track whether any swaps are made in a pass

    // Iterate through the array
    for (int i = 0; i < n - 1; i++)
    {
        swapped = false; // Reset the swapped flag for this pass

        // Iterate through the unsorted part of the array
        for (int j = 0; j < n - i - 1; j++)
        {
            // If the current element is greater than the next element, swap them
            if (values[j] > values[j + 1])
            {
                int temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
                swapped = true; // Set the swapped flag to indicate a swap occurred
            }
        }

        // If no swaps were made in this pass, the array is already sorted
        if (!swapped)
        {
            break; // Exit the loop early, as no further passes are needed
        }
    }

    // Return statement was removed as it served no purpose this time. The same will be applied to selection and insertion sort.
}

// Sorts array of n values using selection sort
void selection(int values[], int n)
{
    // Iterate through the array
    for (int i = 0; i < n - 1; i++)
    {
        int min_target = i; // Assume the current index is the minimum

        // Iterate through the unsorted part of the array to find the smallest element
        for (int j = i + 1; j < n; j++)
        {
            // If the current element is smaller than the assumed minimum, update the index
            if (values[j] < values[min_target])
            {
                min_target = j;
            }
        }

        // Swap the found minimum element with the first element of the unsorted sublist
        int temporal_storage = values[min_target];
        values[min_target] = values[i];
        values[i] = temporal_storage;
    }
}

// Sorts array of n values using insertion sort
void insertion(int values[], int n)
{
    int temp, inner_index;

    // Iterate through the array starting from the second element
    for (int i = 1; i < n; i++)
    {
        temp = values[i]; // Store the current element
        inner_index = i; // Initialize the index for comparisons

        // Compare the current element with the preceding elements and move larger ones to the right
        while (inner_index != 0 && temp < values[inner_index - 1])
        {
            --inner_index;
            values[inner_index + 1] = values[inner_index]; // Shift elements to the right
        }

        values[inner_index] = temp; // Place the current element in its correct position
    }
}
