#include <stdio.h>

int main() {
    int arr[3] = {10, 20, 30};
    int *ptr = arr; // ptr points to arr[0]

    printf("--- Part 1: Pointer Arithmetic ---\n");
    printf("Address of arr[0]: %p, Value: %d\n", (void*)&arr[0], arr[0]);
    printf("Address of ptr:    %p, Value: %d\n", (void*)ptr, *ptr);
    
    ptr++; // Move to next int element (shifts by sizeof(int) bytes)
    printf("After ptr++, Address: %p, Value: %d\n", (void*)ptr, *ptr);

    // Questionable operation: What happens if we do arr++?
    // arr++; // ERROR: array name is a constant pointer, cannot be modified.

    printf("\n--- Part 2: Pointer vs Array Name ---\n");
    printf("sizeof(arr): %lu bytes\n", sizeof(arr)); // Returns total size of array
    printf("sizeof(ptr): %lu bytes\n", sizeof(ptr)); // Returns size of pointer variable

    printf("\n--- Part 3: Const Pointer Variations ---\n");
    int val1 = 100;
    int val2 = 200;

    // Case A: Pointer to Constant
    const int *ptr_to_const = &val1;
    ptr_to_const = &val2; // OK: address can be changed
    // *ptr_to_const = 150; // ERROR: assignment of read-only location

    // Case B: Constant Pointer
    int * const const_ptr = &val1;
    *const_ptr = 150; // OK: value can be changed
    // const_ptr = &val2; // ERROR: assignment of read-only variable

    printf("val1 new value: %d\n", val1);
    printf("ptr_to_const points to value: %d\n", *ptr_to_const);

    return 0;
}