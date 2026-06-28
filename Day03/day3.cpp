#include <stdio.h>
#include <stdlib.h>

// Function using Double Pointer to allocate memory for main
void allocate_memory(int **pptr) {
    *pptr = (int*)malloc(sizeof(int)); // Modifies the pointer in main directly
    if (*pptr != NULL) {
        **pptr = 42; 
    }
}

int main() {
    printf("--- Part 1: Pass by Double Pointer ---\n");
    int *my_ptr = NULL;
    
    // Pass the ADDRESS of my_ptr (which is an int**)
    allocate_memory(&my_ptr); 
    
    if (my_ptr != NULL) {
        printf("Successfully allocated! Value: %d\n", *my_ptr);
        free(my_ptr);
        my_ptr = NULL;
    }

    printf("\n--- Part 2: 2D Array Memory Continuity ---\n");
    int arr[2][3] = {{10, 20, 30}, {40, 50, 60}};
    
    printf("Address of arr[0][0]: %p\n", (void*)&arr[0][0]);
    printf("Address of arr[0][1]: %p (Shifts by %lu bytes)\n", (void*)&arr[0][1], sizeof(int));
    printf("Address of arr[0]   : %p\n", (void*)arr[0]);
    printf("Address of arr[1]   : %p (Shifts by 1 whole row: %lu bytes)\n", (void*)arr[1], 3 * sizeof(int));

    return 0;
}