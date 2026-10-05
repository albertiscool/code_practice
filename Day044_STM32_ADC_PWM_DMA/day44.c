#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// 題目 1: DMA 乒乓雙緩衝 (Ping-Pong Double Buffering) 模擬
// ============================================================================
#define DMA_BUF_SIZE 1000
#define HALF_BUF_SIZE (DMA_BUF_SIZE / 2)

typedef struct {
    uint16_t buffer[DMA_BUF_SIZE];
    bool half_transfer_flag;     // HT 中斷旗標 (0~499 滿)
    bool transfer_complete_flag; // TC 中斷旗標 (500~999 滿)
} Mock_DMA_Buffer_t;

static Mock_DMA_Buffer_t g_dma = {0};

void dma_half_transfer_isr(void) {
    printf("[DMA ISR] Half-Transfer Interrupt (HT)! Buffer A (0~499) Filled.\n");
    printf("  ↳ CPU Processing Buffer A (0~499) while DMA writes to Buffer B (500~999)\n");
    g_dma.half_transfer_flag = false;
}

void dma_transfer_complete_isr(void) {
    printf("[DMA ISR] Transfer-Complete Interrupt (TC)! Buffer B (500~999) Filled.\n");
    printf("  ↳ CPU Processing Buffer B (500~999) while DMA loops back to Buffer A (0~499)\n");
    g_dma.transfer_complete_flag = false;
}

// ============================================================================
// 題目 2: 12-bit ADC 類比電壓換算
// ============================================================================
float convert_adc_to_voltage(uint16_t adc_val, float vref) {
    return vref * ((float)adc_val / 4095.0f);
}

// ============================================================================
// 題目 3: PWM CNT / CCR 定時器比較暫存器計算
// ============================================================================
typedef struct {
    uint32_t ARR; // Auto-Reload Register (週期上限)
    uint32_t CCR; // Capture/Compare Register (比較門檻)
} Mock_TIM_TypeDef;

float calculate_pwm_duty_cycle(Mock_TIM_TypeDef *tim) {
    return ((float)tim->CCR / (float)tim->ARR) * 100.0f;
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 44: STM32 ADC, PWM & DMA Double Buffering Test ---\n");
    printf("=================================================================\n\n");

    // 測試 1: DMA 雙緩衝中斷模擬
    printf("--- 1. DMA Circular Double Buffering Simulation ---\n");
    dma_half_transfer_isr();
    dma_transfer_complete_isr();

    // 測試 2: ADC 12-bit 電壓換算
    printf("\n--- 2. ADC 12-bit Voltage Conversion ---\n");
    uint16_t adc_raw = 2048; // 12-bit 中間值
    float vref = 3.3f;
    float measured_v = convert_adc_to_voltage(adc_raw, vref);
    printf("↳ ADC Raw Value: %d (12-bit) -> Measured Voltage: %.2f V (Expected: 1.65 V)\n", adc_raw, measured_v);

    // 測試 3: PWM 定時器 CCR 暫存器調速
    printf("\n--- 3. PWM Timer CNT/CCR Duty Cycle Calculation ---\n");
    Mock_TIM_TypeDef tim2 = { .ARR = 1000, .CCR = 250 }; // 初始 25% Duty Cycle
    printf("↳ Initial PWM Duty Cycle (CCR = %d, ARR = %d): %.1f%%\n", 
           tim2.CCR, tim2.ARR, calculate_pwm_duty_cycle(&tim2));

    // 增大 CCR 以提高馬達轉速 (增加佔空比)
    tim2.CCR = 750;
    printf("↳ Increased CCR to %d -> Updated Duty Cycle: %.1f%% (Motor Speed Increased!)\n", 
           tim2.CCR, calculate_pwm_duty_cycle(&tim2));

    printf("\n=================================================================\n");
    return 0;
}
