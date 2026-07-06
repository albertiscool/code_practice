#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// --- Mocking Hardware UART Registers ---
typedef struct {
    volatile uint32_t SR;   // Status Register (Holds TXE, RXNE flags)
    volatile uint32_t DR;   // Data Register (Holds actual byte to send/receive)
} UART_TypeDef;

// Bit Definitions for Status Register
#define UART_SR_RXNE  (1 << 5) // Bit 5: Read data register not empty
#define UART_SR_TXE   (1 << 7) // Bit 7: Transmit data register empty

// Global mock UART peripheral instance
UART_TypeDef Mock_UART = { .SR = UART_SR_TXE, .DR = 0 }; 

// --- Core Function 1: Hardware-level Byte Transmission (Polling) ---
void UART_Transmit_Byte(uint8_t byte) {
    // Wait until Transmit Data Register is Empty (TXE == 1)
    // If hardware is still shifting out previous byte, we must block and loop
    while ((Mock_UART.SR & UART_SR_TXE) == 0) {
        // High-speed polling block
    }

    // Write byte into the Data Register (This triggers physical wave shifting out)
    Mock_UART.DR = byte;
    
    // Simulate hardware behavior: clear TXE temporarily while sending
    Mock_UART.SR &= ~UART_SR_TXE; 
    printf("[Hardware TX Line] Sending Waveform for Character: '%c' (0x%02X)\n", byte, byte);
    
    // Simulate hardware completing transfer instantly in software mock
    Mock_UART.SR |= UART_SR_TXE; 
}

// --- Core Function 2: Hardware-level Byte Reception (Polling) ---
uint8_t UART_Receive_Byte(void) {
    // Wait until Read Data Register is Not Empty (RXNE == 1)
    // Loop blocks here until external device drives the RX line down
    while ((Mock_UART.SR & UART_SR_RXNE) == 0) {
        // Polling loop waiting for data
    }

    // Read the data byte from Data Register (This automatically clears RXNE in real hardware)
    uint8_t received_data = (uint8_t)(Mock_UART.DR & 0xFF);
    
    Mock_UART.SR &= ~UART_SR_RXNE; // Clear flag in our mock
    return received_data;
}

int main() {
    printf("--- Part 1: Simulating Polling Transmission ---\n");
    char *message = "MCU";
    for (int i = 0; message[i] != '\0'; i++) {
        UART_Transmit_Byte(message[i]);
    }

    printf("\n--- Part 2: Simulating Data Arrival ---\n");
    // Force inject data into hardware mock to simulate external device sending 'A'
    Mock_UART.DR = 'A';
    Mock_UART.SR |= UART_SR_RXNE; // Trigger RXNE flag

    printf("Polling RX Line...\n");
    uint8_t data = UART_Receive_Byte();
    printf("Successfully captured data from RX Line: '%c'\n", data);

    return 0;
}