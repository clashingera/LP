//2A
#include <iostream>     // For standard I/O operations
#include <cstdlib>      // For general-purpose functions (optional here)
#include <omp.h>        // For OpenMP parallel programming
using namespace std;

// Function declarations
void bubble(int *, int);         // Function to perform parallel bubble sort
void swap(int &, int &);         // Function to swap two integers

// Function to perform parallel bubble sort using odd-even transposition logic
void bubble(int *a, int n)
{
    // Loop n times (worst case for bubble sort to fully sort the array)
    for (int i = 0; i < n; i++)
    {
        // Decide starting index based on iteration (even or odd phase)
        int first = i % 2;  // If i is even, start from 0; if odd, start from 1

        // Parallelize this loop using OpenMP
        // Threads will work on non-overlapping pairs (a[j], a[j+1])
#pragma omp parallel for shared(a, first)
        for (int j = first; j < n - 1; j += 2) // Increment by 2 to handle pairs
        {
            if (a[j] > a[j + 1])              // If pair is out of order
            {
                swap(a[j], a[j + 1]);         // Swap the elements
            }
        }
    }
}

// Swap function to exchange values of two variables using references
void swap(int &a, int &b)
{
    int temp;        // Temporary variable to hold one value
    temp = a;
    a = b;
    b = temp;
}

// Main function
int main()
{
    int *a, n;                             // Pointer for dynamic array and variable for size

    cout << "\nEnter total number of elements: ";
    cin >> n;                             // Input number of elements

    a = new int[n];                       // Dynamically allocate array of size n

    cout << "\nEnter elements: ";
    for (int i = 0; i < n; i++)           // Input array elements
    {
        cin >> a[i];
    }

    bubble(a, n);                         // Call the parallel bubble sort function

    cout << "\nSorted array is:\n";
    for (int i = 0; i < n; i++)           // Output the sorted array
    {
        cout << a[i] << " ";
    }
    cout << endl;

    delete[] a;                           // Free dynamically allocated memory
    return 0;                             // Exit the program
}


// Run Commands:
// g++ -fopenmp -o bubble_sort 3A_bublesort.cpp
// ./bubble_sort
