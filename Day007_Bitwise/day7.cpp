#include <stdio.h>
#include <stdint.h>

// Helper function to print a byte in binary format for debugging
void print_binary8(uint8_t num) {
    printf("0b");
    for (int i = 7; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

int main() {
    uint8_t virtual_register = 0b00000000; // Initialize all bits to 0
    printf("Initial Register state:      ");
    print_binary8(virtual_register);

    printf("\n--- Part 1: Set Bit 3 ---\n");
    virtual_register |= (1 << 3); // Set bit 3 to 1
    printf("After Setting Bit 3:         ");
    print_binary8(virtual_register);

    printf("\n--- Part 2: Toggle Bit 7 ---\n");
    virtual_register ^= (1 << 7); // Toggle bit 7 (0 becomes 1)
    printf("After Toggling Bit 7:        ");
    print_binary8(virtual_register);

    printf("\n--- Part 3: Check Bit 3 State ---\n");
    if (virtual_register & (1 << 3)) {
        printf("Status: Bit 3 is HIGH (1)\n");
    } else {
        printf("Status: Bit 3 is LOW (0)\n");
    }

    printf("\n--- Part 4: Clear Bit 3 ---\n");
    virtual_register &= ~(1 << 3); // Clear bit 3 back to 0
    printf("After Clearing Bit 3:        ");
    print_binary8(virtual_register);

    return 0;
}