#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// --- Mocking Hardware GPIO Pins for SPI ---
void Set_CS_Low()  { printf("[CS -> 0] Select Slave\n"); }
void Set_CS_High() { printf("[CS -> 1] Deselect Slave\n"); }

// --- Core Function: SPI Full-Duplex Byte Exchange ---
// In SPI, you ALWAYS write and read a byte simultaneously
uint8_t SPI_Exchange_Byte(uint8_t master_tx_data) {
    uint8_t master_rx_data = 0;
    
    // Mocking a dummy Slave Shift Register holding data 0x3F (0b00111111)
    uint8_t slave_tx_data = 0x3F; 

    // Shift out 8 bits, MSB first
    for (int i = 7; i >= 0; i--) {
        // 1. Master places bit onto MOSI line
        uint8_t mosi_bit = (master_tx_data >> i) & 1;
        
        // 2. Slave places bit onto MISO line
        uint8_t miso_bit = (slave_tx_data >> i) & 1;

        // 3. Clock Pulses (Sampling occurs here)
        // Master samples MISO bit and shifts it into master_rx_data
        master_rx_data |= (miso_bit << i);

        // (In real hardware, slave would also sample MOSI here)
    }

    return master_rx_data;
}

int main() {
    printf("--- Starting SPI Communication ---\n");
    
    // Step 1: Pull CS Low to activate the slave device
    Set_CS_Low();

    // Step 2: Send Command 0x9F (Commonly used to read Flash Memory ID)
    // We send 0x9F, and we simultaneously capture whatever the slave shifts back
    uint8_t send_val = 0x9F;
    printf("\n[Master TX] Sending command: 0x%02X\n", send_val);
    
    uint8_t received_val = SPI_Exchange_Byte(send_val);
    
    printf("[Master RX] Simultaneously captured from Slave: 0x%02X\n\n", received_val);

    // Step 3: Pull CS High to end communication
    Set_CS_High();

    return 0;
}