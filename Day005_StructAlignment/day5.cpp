#include <stdio.h>

// Struct A: Bad alignment order (wastes memory)
typedef struct {
    char a;      // 1 byte
                 // 3 bytes padding here! (to align 'b' to a 4-byte boundary)
    int b;       // 4 bytes
    char c;      // 1 byte
                 // 3 bytes padding here at the end! (to make total size a multiple of 4)
} StructA_t;

// Struct B: Optimized alignment order (saves memory)
typedef struct {
    int b;       // 4 bytes (starts at 0)
    char a;      // 1 byte  (starts at 4)
    char c;      // 1 byte  (starts <at 5)
                 // 2 bytes padding here at the end! (to make total size a multiple of 4)
} StructB_t;

int main() {
    printf("--- Part 1: Sizeof Comparison ---\n");
    printf("Size of StructA (Unoptimized): %lu bytes\n", sizeof(StructA_t));
    printf("Size of StructB (Optimized):   %lu bytes\n", sizeof(StructB_t));

    printf("\n--- Part 2: Detailed Member Memory Layout (Struct A) ---\n");
    StructA_t objA;
    printf("Address of objA:   %p\n", (void*)&objA);
    printf("Address of objA.a: %p (char, 1 byte)\n", (void*)&objA.a);
    printf("Address of objA.b: %p (int,  4 bytes)\n", (void*)&objA.b);
    printf("Address of objA.c: %p (char, 1 byte)\n", (void*)&objA.c);

    printf("\n--- Part 3: Detailed Member Memory Layout (Struct B) ---\n");
    StructB_t objB;
    printf("Address of objB:   %p\n", (void*)&objB);
    printf("Address of objB.b: %p (int,  4 bytes)\n", (void*)&objB.b);
    printf("Address of objB.a: %p (char, 1 byte)\n", (void*)&objB.a);
    printf("Address of objB.c: %p (char, 1 byte)\n", (void*)&objB.c);

    return 0;
}