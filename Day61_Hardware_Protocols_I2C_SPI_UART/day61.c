#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// 1. I2C 9-Clock 總線死鎖復位模擬 (9-Clock Bus Recovery SOP)
// ============================================================================
void demo_i2c_bus_recovery(void) {
    printf("--- 1. I2C Bus Deadlock & 9-Clock Recovery Simulation ---\n");
    printf("↳ Scenario: Master resets mid-read while Slave holds SDA LOW (0V).\n");
    printf("↳ Recovery Step 1: Switch SCL pin to GPIO Output mode.\n");
    printf("↳ Recovery Step 2: Toggle SCL 9 times (Clock out unread bits)...\n");
    for (int i = 1; i <= 9; i++) {
        printf("   [Clock %d] SCL: HIGH -> LOW -> HIGH\n", i);
    }
    printf("↳ Recovery Step 3: Slave sees no ACK on 9th clock and releases SDA to HIGH (3.3V/5V)!\n");
    printf("↳ Recovery Step 4: Master issues a standard STOP Condition (SCL HIGH, SDA LOW->HIGH).\n");
    printf("↳ Result: I2C Bus successfully UNLOCKED!\n\n");
}

// ============================================================================
// 2. SPI 4 種模式 (CPOL/CPHA) 判定與採樣邊緣計算
// ============================================================================
void print_spi_mode_info(int mode) {
    int cpol = (mode >> 1) & 1;
    int cpha = mode & 1;
    
    printf("SPI Mode %d (CPOL=%d, CPHA=%d):\n", mode, cpol, cpha);
    printf("  - Idle Clock Level : %s\n", cpol ? "HIGH (3.3V)" : "LOW (0V)");
    
    const char *sample_edge = "";
    if (cpol == 0 && cpha == 0) sample_edge = "Rising Edge (第 1 邊緣)";
    else if (cpol == 0 && cpha == 1) sample_edge = "Falling Edge (第 2 邊緣)";
    else if (cpol == 1 && cpha == 0) sample_edge = "Falling Edge (第 1 邊緣)";
    else if (cpol == 1 && cpha == 1) sample_edge = "Rising Edge (第 2 邊緣)";
    
    printf("  - Data Sample Edge : %s\n", sample_edge);
}

void demo_spi_modes(void) {
    printf("--- 2. SPI 4-Mode Hardware Characteristics ---\n");
    for (int m = 0; m < 4; m++) {
        print_spi_mode_info(m);
    }
    printf("↳ Bus Sharing: SCK, MOSI, MISO are shared in parallel. CS is pulled LOW per device.\n\n");
}

// ============================================================================
// 3. UART 傳輸耗時與 16 倍過採樣分頻計算
// ============================================================================
void demo_uart_baud_and_timing(void) {
    uint32_t baud = 115200;
    uint32_t bytes_to_send = 1152;
    
    // 標準 8N1 格式: 1 Start + 8 Data + 0 Parity + 1 Stop = 10 bits per byte
    uint32_t total_bits = bytes_to_send * 10;
    float time_ms = ((float)total_bits / (float)baud) * 1000.0f;
    
    printf("--- 3. UART Transmission Time & 16x Oversampling Math ---\n");
    printf("↳ Baud Rate: %u bps (8N1 Format = 10 bits/byte)\n", baud);
    printf("↳ 1 Byte Transmission Time = 10 / %u = %.2f us\n", baud, (10.0f / (float)baud) * 1000000.0f);
    printf("↳ %u Bytes Packet Transmission Time = %u bits / %u = %.2f ms\n", bytes_to_send, total_bits, baud, time_ms);
    
    // 晶振分頻計算 N = f_osc / (16 * Baud)
    uint32_t f_osc_magic = 11059200; // 11.0592 MHz
    uint32_t f_osc_round = 12000000; // 12.0000 MHz
    
    float div_magic = (float)f_osc_magic / (16.0f * (float)baud);
    float div_round = (float)f_osc_round / (16.0f * (float)baud);
    
    printf("\n↳ Oscillator Divider Math (N = f_osc / (16 * Baud)):\n");
    printf("   [11.0592 MHz Crystal] N = %u / (16 * %u) = %.4f (Exact Integer 6 -> 0.00%% Error!)\n", f_osc_magic, baud, div_magic);
    printf("   [12.0000 MHz Crystal] N = %u / (16 * %u) = %.4f (Non-integer -> ~8.0%% Error Corrupts Data!)\n", f_osc_round, baud, div_round);
}

// ============================================================================
// Main 測試程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 61: Hardware Protocols - I2C, SPI & UART Deep Dive ---\n");
    printf("=================================================================\n\n");

    demo_i2c_bus_recovery();
    demo_spi_modes();
    demo_uart_baud_and_timing();

    printf("=================================================================\n");
    printf("🎉 CONGRATULATIONS! STAGE 1 (I2C, SPI, UART) 100%% MASTERED! 🎉\n");
    printf("=================================================================\n");
    return 0;
}
