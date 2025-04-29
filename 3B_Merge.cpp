//3_merge
#include <iostream>      // For standard input/output
#include <stdlib.h>      // For general utilities (optional here)
#include <omp.h>         // For OpenMP parallelism
using namespace std;

// Function declarations
void mergesort(int a[], int i, int j);                   // Recursive merge sort
void merge(int a[], int i1, int j1, int i2, int j2);     // Merge two sorted parts

// Merge Sort Function using OpenMP sections
void mergesort(int a[], int i, int j)
{
    int mid;
    if (i < j)                         // Base condition for recursion
    {
        mid = (i + j) / 2;             // Find mid point to divide the array

        // Parallel sections to sort left and right halves concurrently
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergesort(a, i, mid);      // Recursive sort on left half
            }

            #pragma omp section
            {
                mergesort(a, mid + 1, j);  // Recursive sort on right half
            }
        }

        merge(a, i, mid, mid + 1, j);      // Merge the two sorted halves
    }
}

// Function to merge two sorted sub-arrays a[i1..j1] and a[i2..j2]
void merge(int a[], int i1, int j1, int i2, int j2)
{
    int temp[1000];      // Temporary array to hold merged result
    int i = i1;          // Pointer for first sub-array
    int j = i2;          // Pointer for second sub-array
    int k = 0;           // Index for temp array

    // Merge elements from both sub-arrays in sorted order
    while (i <= j1 && j <= j2)
    {
        if (a[i] < a[j])
        {
            temp[k++] = a[i++];
        }
        else
        {
            temp[k++] = a[j++];
        }
    }

    // Copy remaining elements of first sub-array if any
    while (i <= j1)
    {
        temp[k++] = a[i++];
    }

    // Copy remaining elements of second sub-array if any
    while (j <= j2)
    {
        temp[k++] = a[j++];
    }

    // Copy sorted elements back to original array
    for (i = i1, j = 0; i <= j2; i++, j++)
    {
        a[i] = temp[j];
    }
}

// Main function
int main()
{
    int *a, n, i;

    cout << "\nEnter total number of elements => ";
    cin >> n;                    // Take number of elements from user
    a = new int[n];             // Dynamically allocate array of size n

    cout << "\nEnter elements => ";
    for (i = 0; i < n; i++)     // Input array elements
    {
        cin >> a[i];
    }

    // Measure start time of parallel merge sort
    double start = omp_get_wtime();

    mergesort(a, 0, n - 1);     // Perform merge sort on entire array

    // Measure stop time
    double stop = omp_get_wtime();

    // Print the sorted array
    cout << "\nSorted array is => ";
    for (i = 0; i < n; i++)
    {
        cout << "\n" << a[i];
    }

    // Print total execution time
    cout << "\n\nTime taken by parallel merge sort: " << stop - start << " seconds\n";

    return 0;
}


// Run Commands:
// g++ -fopenmp -o merge_sort 3B_Merge.cpp
// ./merge_sort