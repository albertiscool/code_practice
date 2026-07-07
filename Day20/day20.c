#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// --- Mocking Microcontroller GPIO Register Control ---
// In real hardware, these would modify GPIOx->ODR registers
void Set_SCL_High() { printf("[SCL -> 1] "); }
void Set_SCL_Low()  { printf("[SCL -> 0] "); }
void Set_SDA_High() { printf("[SDA -> 1] \n"); }
void Set_SDA_Low()  { printf("[SDA -> 0] \n"); }
bool Read_SDA_Input() { return false; // Mocking slave returning ACK (0)
}

// --- Core Function 1: Generate I2C START Condition ---
void I2C_Start(void) {
    printf("--- Generating START Condition ---\n");
    Set_SDA_High();
    Set_SCL_High();
    // SDA falling edge while SCL is High triggers START
    Set_SDA_Low();
    Set_SCL_Low(); // Clamp SCL low to prepare for data shifting
}

// --- Core Function 2: Generate I2C STOP Condition ---
void I2C_Stop(void) {
    printf("--- Generating STOP Condition ---\n");
    Set_SDA_Low();
    Set_SCL_High();
    // SDA rising edge while SCL is High triggers STOP
    Set_SDA_High();
}

// --- Core Function 3: Write 1 Byte over I2C and Check ACK ---
bool I2C_Write_Byte(uint8_t byte) {
    printf("--- Shifting Out Byte: 0x%02X ---\n", byte);
    
    // Shift out 8 bits, MSB (Most Significant Bit) first
    for (int i = 7; i >= 0; i--) {
        uint8_t bit = (byte >> i) & 1;
        
        if (bit) Set_SDA_High();
        else     Set_SDA_Low();
        
        // Pulse the clock to let slave sample the data bit
        Set_SCL_High();
        Set_SCL_Low(); // Data can change only when SCL is low
    }
    
    // --- 9th Clock Cycle: Handle ACK/NACK ---
    Set_SDA_High(); // Release SDA line so slave can control it
    Set_SCL_High(); // Master drives SCL high for slave acknowledgement
    
    printf("[9th Clock] Sampling ACK... ");
    bool ack = !Read_SDA_Input(); // If SDA is driven LOW by slave, it is an ACK
    printf(ack ? "ACK Received!\n" : "NACK Error!\n");
    
    Set_SCL_Low(); // Finish the clock pulse
    return ack;
}

int main() {
    // Simulating a standard write sequence to a slave register
    // Example: Writing data 0x55 to Slave Address 0xA0
    I2C_Start();
    
    if (I2C_Write_Byte(0xA0)) { // 1. Send Slave Address + Write bit
        I2C_Write_Byte(0x55);   // 2. Send Data Byte
    }
    
    I2C_Stop();
    return 0;
}