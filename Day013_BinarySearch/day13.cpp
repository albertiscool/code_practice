#include <stdio.h>

// --- Core Interview Question: Safe Binary Search ---
// Returns the index of the target if found, otherwise returns -1
int binary_search(int arr[], int size, int target) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        // OVERFLOW SAFE METHOD (Crucial for firmware interviews!)
        int mid = low + (high - low) / 2; 

        printf("[Debugging] Checking Index mid: %d (Value: %d) | range: [%d, %d]\n", mid, arr[mid], low, high);

        if (arr[mid] == target) {
            return mid; // Target found, return index
        }
        
        if (target < arr[mid]) {
            high = mid - 1; // Target is in the left half
        } else {
            low = mid + 1;  // Target is in the right half
        }
    }

    return -1; // Target not found
}

int main() {
    // Binary search REQUIRES a sorted array!
    int sorted_array[] = {11, 22, 33, 44, 55, 66, 77, 88, 99};
    int size = sizeof(sorted_array) / sizeof(sorted_array[0]);
    
    int target1 = 77;
    int target2 = 100; // Edge case: item not in array

    printf("--- Search Case 1: Finding %d ---\n", target1);
    int result1 = binary_search(sorted_array, size, target1);
    if (result1 != -1) {
        printf("Success: Target %d found at array index [%d]\n", target1, result1);
    } else {
        printf("Failed: Target %d not found.\n", target1);
    }

    printf("\n--- Search Case 2: Finding %d (Not Exist) ---\n", target2);
    int result2 = binary_search(sorted_array, size, target2);
    if (result2 != -1) {
        printf("Success: Target %d found at array index [%d]\n", target2, result2);
    } else {
        printf("Failed: Target %d not found.\n", target2);
    }

    return 0;
}