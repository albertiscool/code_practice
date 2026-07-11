#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// --- Mocking DMA Channel Registers ---
typedef struct {
    volatile uint32_t CPAR;  // Peripheral Address Register (Source)
    volatile uint32_t CMAR;  // Memory Address Register     (Destination)
    volatile uint32_t CNDTR; // Number of Data to Transfer Register
    volatile uint32_t CCR;   // Configuration Register (Enable, Direction, Increment)
} DMA_Channel_TypeDef;

#define DMA_CCR_EN     (1 << 0) // Bit 0: Channel enable
#define DMA_CCR_MINC   (1 << 7) // Bit 7: Memory increment mode (Auto pointer++)

// Global mock UART Data Register address
uint8_t Mock_UART_DR = 0;

// Global instance of DMA1 Channel 1
DMA_Channel_TypeDef DMA1_Channel1 = {0};

// --- Core Function: Simulate Background DMA Hardware Transfer ---
// In real hardware, this is triggered automatically by a peripheral hardware signal (request)
void Simulate_DMA_Hardware_Tick(void) {
    // Check if DMA channel is active and has items left to transfer
    if ((DMA1_Channel1.CCR & DMA_CCR_EN) && (DMA1_Channel1.CNDTR > 0)) {
        
        // 1. Dereference the pointers set in the registers
        uint8_t *src_ptr = (uint8_t *)DMA1_Channel1.CPAR;
        uint8_t *dest_ptr = (uint8_t *)DMA1_Channel1.CMAR;
        
        // 2. Hardware performs the copy directly on the bus (Bypassing CPU!)
        *dest_ptr = *src_ptr;
        
        printf("[DMA Hardware] Copied byte '0x%02X' from 0x%p to 0x%p | Left: %d\n", 
               *src_ptr, src_ptr, dest_ptr, DMA1_Channel1.CNDTR - 1);
        
        // 3. Address Increment Logic
        if (DMA1_Channel1.CCR & DMA_CCR_MINC) {
            DMA1_Channel1.CMAR++; // Auto increment the destination RAM pointer
        }
        
        // 4. Decrement the counter
        DMA1_Channel1.CNDTR--;
        
        // 5. Transfer Complete Check
        if (DMA1_Channel1.CNDTR == 0) {
            DMA1_Channel1.CCR &= ~DMA_CCR_EN; // Disable channel
            printf("====== [DMA Interrupt] Transfer Complete! (TC Flag Set) ======\n");
        }
    }
}

int main() {
    uint8_t my_ram_buffer[3] = {0}; // Our destination buffer in RAM

    printf("--- Step 1: Configuring DMA Registers ---\n");
    DMA1_Channel1.CPAR  = (uint32_t)&Mock_UART_DR;       // Source: UART Data Reg
    DMA1_Channel1.CMAR  = (uint32_t)&my_ram_buffer[0];   // Destination: RAM array
    DMA1_Channel1.CNDTR = 3;                             // Transfer size = 3 bytes
    DMA1_Channel1.CCR  |= (DMA_CCR_EN | DMA_CCR_MINC);   // Enable DMA and Memory Increment

    printf("--- Step 2: Simulating Data Stream Arrival ---\n");
    
    // Byte 1 arrives at peripheral
    Mock_UART_DR = 'M';
    Simulate_DMA_Hardware_Tick(); // Triggered by hardware request
    
    // Byte 2 arrives at peripheral
    Mock_UART_DR = 'C';
    Simulate_DMA_Hardware_Tick();
    
    // Byte 3 arrives at peripheral
    Mock_UART_DR = 'U';
    Simulate_DMA_Hardware_Tick();

    printf("\n--- Step 3: Verifying RAM Contents (CPU checks results) ---\n");
    printf("Buffer holds string: %c%c%c\n", my_ram_buffer[0], my_ram_buffer[1], my_ram_buffer[2]);

    return 0;
}