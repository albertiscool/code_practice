#include <stdio.h>
#include <stdlib.h>

int global_init_var = 10;     // Initialized Global Variable (Stored in Data Segment)
int global_uninit_var;        // Uninitialized Global Variable (Stored in BSS Segment)

void test_function(int arg) {
    int local_var = 5;        // Local Variable (Stored in Stack Frame)
    printf("--- Stack Segment ---\n");
    printf("Address of function argument 'arg':    %p\n", (void*)&arg);
    printf("Address of local variable 'local_var': %p\n", (void*)&local_var);
}

int main() {
    int main_local = 20;      // Local Variable in main (Stored in Stack)
    
    // Dynamically allocate memory from Heap
    int *heap_var = (int*)malloc(sizeof(int)); 
    
    if (heap_var == NULL) {   // Always check if malloc succeeded
        return -1;
    }
    
    printf("--- Data / BSS Segment ---\n");
    printf("Address of initialized global var:     %p\n", (void*)&global_init_var);
    printf("Address of uninitialized global var:   %p\n", (void*)&global_uninit_var);
    
    printf("\n--- Heap Segment ---\n");
    printf("Address allocated on Heap (heap_var):  %p\n", (void*)heap_var);
    
    printf("\n--- Stack Segment ---\n");
    printf("Address of local var in main:          %p\n", (void*)&main_local);
    
    test_function(main_local);
    
    free(heap_var);          // Free the allocated memory to prevent memory leaks
    heap_var = NULL;          // Avoid dangling pointer
    
    return 0;
}