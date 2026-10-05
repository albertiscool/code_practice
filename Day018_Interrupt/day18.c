#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// --- Global Hardware Shared Variables ---
// MUST use volatile because this flag is modified inside the ISR 
// and monitored inside the infinite main loop!
volatile bool g_button_pressed_flag = false;
volatile uint32_t g_interrupt_counter = 0;

// --- Mock Hardware Interrupt Service Routine (ISR) ---
// In a real STM32, this name corresponds to the vector table (e.g., EXTI0_IRQHandler)
void EXTI0_IRQHandler(void) {
    // Step 1: Check if the specific interrupt flag is set (Hardware specific)
    // [Mocked] Assume hardware triggered this block
    
    // Step 2: Perform MINIMAL and FAST operations
    g_button_pressed_flag = true;
    g_interrupt_counter++;

    // Step 3: Crucial! Clear the hardware pending interrupt flag
    // If you forget to clear it, the MCU will re-trigger the ISR infinitely!
    // [Mocked] EXTI->PR |= (1 << 0); 
}

// --- Main Processing Loop (Background Task) ---
int main(void) {
    printf("--- MCU System Initialized ---\n");
    printf("Waiting for hardware interrupt events...\n\n");

    // Simulating an infinite loop in firmware
    int execution_ticks = 0;
    while (execution_ticks < 5) {
        
        // Simulating a hardware event happening randomly at tick 2
        if (execution_ticks == 2) {
            printf("[Hardware Event] User pushes the physical button!\n");
            EXTI0_IRQHandler(); // Hardware forces CPU to jump into ISR
        }

        // Main loop polls the volatile flag (Deferring heavy tasks here)
        if (g_button_pressed_flag) {
            printf("[Main Loop Processing] Deferring heavy task here...\n");
            printf("Processing Button Event #%d: Updating LED states onto screen.\n", g_interrupt_counter);
            
            // Reset the flag after processing the deferred task
            g_button_pressed_flag = false;
        } else {
            printf("[Main Loop] Running low-power background routines...\n");
        }

        execution_ticks++;
    }

    printf("\n--- System Simulation End --- Total Interrupts Handled: %d\n", g_interrupt_counter);
    return 0;
}