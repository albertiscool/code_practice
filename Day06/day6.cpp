#include <stdio.h>

// 1. Using static at global level: This variable is restricted to this file only
static int file_scope_var = 100;

void counter_function(void) {
    int normal_var = 0;
    static int static_var = 0; // Local static: Allocated in Data/BSS segment, initialized ONLY ONCE

    normal_var++;
    static_var++;

    printf("normal_var: %d | static_var: %d\n", normal_var, static_var);
}

int main() {
    printf("--- Part 1: Local Static Variable Demonstration ---\n");
    for (int i = 0; i < 3; i++) {
        counter_function();
    }

    printf("\n--- Part 2: Volatile Concept Demonstration ---\n");
    // Volatile tells compiler: "Hardware may change this value at any microsecond!"
    volatile int hardware_register = 0;
    
    hardware_register = 1;
    hardware_register = 2; // Without volatile, compiler might skip "hardware_register = 1" entirely!
    
    printf("Final register value: %d\n", hardware_register);

    return 0;
}