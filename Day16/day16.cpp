#include <stdint.h>

// --- Pure C Hardware Register Mapping (No Libraries!) ---

// 1. Define the absolute memory addresses of the peripheral registers
#define RCC_AHB1ENR   (*(volatile uint32_t *)0x40023830)
#define GPIOD_MODER   (*(volatile uint32_t *)0x40020C00)
#define GPIOD_ODR     (*(volatile uint32_t *)0x40020C14)

// Simple software delay function
void delay(volatile uint32_t count) {
    while (count--) {
        // Prevent compiler from optimizing out this empty loop
    }
}

int main(void) {
    // STEP 1: Enable Clock for GPIOD (Set Bit 3 of RCC_AHB1ENR to 1)
    RCC_AHB1ENR |= (1 << 3);

    // STEP 2: Configure GPIOD Pin 12 as General Purpose Output Mode
    // Clear bits 25:24 first, then set bit 24 to 1 (01 = Output mode)
    GPIOD_MODER &= ~(3 << 24); // 3 is 0b11, clearing bits 24 and 25
    GPIOD_MODER |= (1 << 24);  // Set bit 24 to 1

    // Infinite Blink Loop
    while (1) {
        // STEP 3: Toggle GPIOD Pin 12 (XOR Bit 12)
        GPIOD_ODR ^= (1 << 12);

        // Delay to make blinking visible
        delay(500000);
    }

    return 0; // Standard C structure, though MCU loops forever
}