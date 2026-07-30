#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// 題目 1: STM32 MMIO 暫存器結構體映射與位址算術
// ============================================================================
typedef struct {
    volatile uint32_t MODER;   // Offset 0x00
    volatile uint32_t OTYPER;  // Offset 0x04
    volatile uint32_t OSPEEDR; // Offset 0x08
    volatile uint32_t PUPDR;   // Offset 0x0C
    volatile uint32_t IDR;     // Offset 0x10 (Input Data)
    volatile uint32_t ODR;     // Offset 0x14 (Output Data)
} Mock_GPIO_TypeDef;

// 模擬 STM32 基底記憶體位址
static Mock_GPIO_TypeDef g_simulated_gpioa_mem = {0};
#define MOCK_GPIOA (&g_simulated_gpioa_mem)

// ============================================================================
// 題目 3: UART ISR 中斷旗標清除模擬
// ============================================================================
typedef struct {
    volatile uint32_t SR;  // Status Register (Bit 5 = RXNE Interrupt Flag)
    volatile uint32_t DR;  // Data Register
} Mock_USART_TypeDef;

static Mock_USART_TypeDef g_simulated_usart1 = { .SR = (1 << 5), .DR = 0x55 };

void USART1_IRQHandler(void) {
    printf("[ISR] UART Interrupt Triggered! Status Register SR = 0x%08X\n", g_simulated_usart1.SR);

    // 檢查 RXNE (Receive Data Register Not Empty) 旗標
    if (g_simulated_usart1.SR & (1 << 5)) {
        uint8_t rx_data = (uint8_t)g_simulated_usart1.DR;
        printf("  ↳ [ISR Read] Captured Data from DR: 0x%02X\n", rx_data);

        // ⭐️ 核心防禦：手動清空中斷旗標 (Clear Interrupt Flag)！
        g_simulated_usart1.SR &= ~(1 << 5);
        printf("  ↳ [ISR Clear] Cleared Interrupt Flag! SR is now: 0x%08X\n", g_simulated_usart1.SR);
    }
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 43: STM32 MMIO, Peripherals, I2C/SPI & NVIC Test ---\n");
    printf("=================================================================\n\n");

    // 測試 1: MMIO 暫存器偏移量與拉高 GPIOA PA5
    printf("--- 1. MMIO Address Calculation & GPIO PA5 Output ---\n");
    printf("↳ Base Address: %p\n", (void*)MOCK_GPIOA);
    printf("↳ ODR Address : %p (Offset: %ld Bytes)\n", 
           (void*)&(MOCK_GPIOA->ODR), 
           (uintptr_t)&(MOCK_GPIOA->ODR) - (uintptr_t)MOCK_GPIOA);

    // 拉高 PA5 腳位 (Bit 5)
    MOCK_GPIOA->ODR |= (1 << 5);
    printf("↳ MOCK_GPIOA->ODR Value after Set Bit 5: 0x%08X (PA5 High)\n", MOCK_GPIOA->ODR);

    // 測試 2: UART ISR 執行與旗標清除驗證
    printf("\n--- 2. UART Interrupt Handler (ISR) Simulation ---\n");
    USART1_IRQHandler();

    printf("\n=================================================================\n");
    return 0;
}
