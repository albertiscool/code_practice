#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// 1. UART 底層波形與暫存器模擬 (UART Data Register & Shift Register)
// ============================================================================
void demo_topic1_uart_hardware(void) {
    char data = 'A'; // ASCII 65 (0x41 / 0b01000001)
    printf("--- 1. UART Hardware Transmission & Registers ---\n");
    printf("↳ Character: '%c' (0x41 / 0b01000001)\n", data);
    printf("↳ Step 1: CPU writes 'A' to UART Data Register (UDR/DR) in 1 clock cycle.\n");
    printf("↳ Step 2: Parallel data loaded into Hardware Shift Register.\n");
    printf("↳ Step 3: Waveform output on TX Pin (Baud 115200 = 8.68 us/bit):\n");
    printf("        StartBit(0V) -> LSB(1) -> 0 -> 0 -> 0 -> 0 -> 1 -> 0 -> MSB(0) -> StopBit(5V)\n");
    printf("↳ Step 4: RX Hardware receives LSB first -> Count 9 = StopBit -> Triggers RX Interrupt (ISR)!\n");
}

// ============================================================================
// 2. DMA 零 CPU 介入傳輸模擬 (Direct Memory Access)
// ============================================================================
void demo_topic2_dma_transfer(void) {
    printf("\n--- 2. DMA (Direct Memory Access) Zero-CPU Overhead ---\n");
    printf("↳ Interrupt Method : 1024 Bytes = 1024 Interrupts (CPU Context Switch Disaster!)\n");
    printf("↳ DMA Method       : 1024 Bytes = 1 Interrupt (Only 1 TC Interrupt at the end!)\n");
    printf("↳ Address Config   : Source (UART DR 0x40001004) = Fixed (No Increment)\n");
    printf("                     Destination (RAM buffer)  = Increment (+1 per byte)\n");
}

// ============================================================================
// 3. PWM 馬達調速與定時器暫存器 (ARR & CCR)
// ============================================================================
void demo_topic3_pwm_timer_registers(void) {
    uint32_t arr = 255; // 決定總週期與頻率
    uint32_t ccr = 127; // 決定高電位占空比
    float duty_cycle = ((float)ccr / (float)arr) * 100.0f;
    float avg_voltage = 5.0f * (duty_cycle / 100.0f);

    printf("\n--- 3. PWM Motor Speed & Timer Registers (ARR & CCR) ---\n");
    printf("↳ Timer ARR (Auto-Reload Register) = %u -> Sets PWM Frequency & Period T\n", arr);
    printf("↳ Timer CCR (Capture/Compare Reg)  = %u -> Sets Duty Cycle & Speed\n", ccr);
    printf("↳ Duty Cycle = %.1f%%, Average Voltage = %.2f V (Speed halved!)\n", duty_cycle, avg_voltage);
}

// ============================================================================
// 4. Linux Shared Memory (mmap) 零拷貝 vs Socket
// ============================================================================
void demo_topic4_zero_copy_shared_memory(void) {
    printf("\n--- 4. Shared Memory (shm_open + mmap) Zero-Copy ---\n");
    printf("↳ Standard ROS Topic / Socket : 2 times memcpy (User -> Kernel -> User)\n");
    printf("↳ Shared Memory (mmap)        : 0 times memcpy (Zero-Copy!)\n");
    printf("↳ Physical Reality: MMU maps Virtual Pages of both Processes to SAME Physical RAM Frame.\n");
}

// ============================================================================
// 5. Multi-task Race Condition & Mutex ARM Atomic Instructions (LDREX / STREX)
// ============================================================================
void demo_topic5_race_condition_atomic(void) {
    printf("\n--- 5. Race Condition & ARM Atomic Instructions ---\n");
    printf("↳ c = count++ in Assembly = 3 Steps: LOAD -> ADD -> STORE\n");
    printf("↳ Without Lock: Preemption interrupts middle step -> Data overwritten & corrupted!\n");
    printf("↳ ARM Hardware Mutex Lock: Powered by LDREX (Load Excl) & STREX (Store Excl) Atomic Instructions.\n");
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 57: Embedded Smart Car Hardware Low-Level Interview ---\n");
    printf("=================================================================\n\n");

    demo_topic1_uart_hardware();
    demo_topic2_dma_transfer();
    demo_topic3_pwm_timer_registers();
    demo_topic4_zero_copy_shared_memory();
    demo_topic5_race_condition_atomic();

    printf("\n=================================================================\n");
    printf("🎉 CONGRATULATIONS! ALL 5 LOW-LEVEL INTERVIEW TOPICS MASTERED! 🎉\n");
    printf("=================================================================\n");
    return 0;
}
