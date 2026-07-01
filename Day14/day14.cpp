#include <stdio.h>

// Helper function to swap two integers using pointers
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// --- Core Partition Function ---
// This function takes the last element as pivot, places the pivot element at its
// correct position in sorted array, and places all smaller elements to left of pivot
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // Choosing the last element as Pivot
    int i = (low - 1);     // Index of smaller element

    for (int j = low; j < high; j++) {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot) {
            i++; // Increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    // Swap the pivot element with the greater element specified by i
    swap(&arr[i + 1], &arr[high]);
    
    return (i + 1); // Return the partitioning index
}

// --- Core Quick Sort Function ---
void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        // pi is partitioning index, arr[pi] is now at right place
        int pi = partition(arr, low, high);

        // Separately sort elements before partition and after partition
        quick_sort(arr, low, pi - 1);  // Left Subarray
        quick_sort(arr, pi + 1, high); // Right Subarray
    }
}

// Helper function to print the array
void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int unsorted_array[] = {64, 25, 12, 22, 11, 90, 45};
    int size = sizeof(unsorted_array) / sizeof(unsorted_array[0]);

    printf("--- Original Unsorted Array ---\n");
    print_array(unsorted_array, size);

    printf("\n--- Executing Quick Sort ---\n");
    quick_sort(unsorted_array, 0, size - 1);

    printf("\n--- Sorted Array Output ---\n");
    print_array(unsorted_array, size);

    return 0;
}