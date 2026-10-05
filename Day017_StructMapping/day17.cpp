#include <stdint.h>

// 1. Define the Hardware Register Layout using a Struct
// The order of members MUST strictly match the reference manual offsets!
typedef struct {
    volatile uint32_t MODER;   // Offset 0x00: Mode register
    volatile uint32_t OTYPER;  // Offset 0x04: Output type register
    volatile uint32_t OSPEEDR; // Offset 0x08: Output speed register
    volatile uint32_t PUPDR;   // Offset 0x0C: Pull-up/pull-down register
    volatile uint32_t IDR;     // Offset 0x10: Input data register
    volatile uint32_t ODR;     // Offset 0x14: Output data register
} GPIO_TypeDef;

// 2. Define Base Addresses
#define RCC_AHB1ENR   (*(volatile uint32_t *)0x40023830)

#define GPIOD_BASE    (0x40020C00)
// Typecast the base address into a pointer to our structure
#define GPIOD         ((GPIO_TypeDef *)GPIOD_BASE)

void delay(volatile uint32_t count) {
    while (count--);
}

int main(void) {
    // Enable GPIOD Clock (Bit 3)
    RCC_AHB1ENR |= (1 << 3);

    // Elegant Struct-style Hardware Access (Just like official SDK!)
    // Configure Pin 12 as output: Clear bits 25:24, then set bit 24
    GPIOD->MODER &= ~(3 << 24);
    GPIOD->MODER |= (1 << 24);

    while (1) {
        // Toggle Pin 12 via the structure pointer
        GPIOD->ODR ^= (1 << 12);
        delay(500000);
    }

    return 0;
}