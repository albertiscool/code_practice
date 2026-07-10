#include <stdio.h>
#include <stdint.h>
#include <unistd.h> // For usleep

// --- Mocking Microcontroller Timer Registers ---
typedef struct {
    volatile uint32_t ARR; // Auto-Reload Register (Defines Period/Frequency)
    volatile uint32_t CCR; // Capture Compare Register (Defines Duty Cycle)
    volatile uint32_t CNT; // Counter Register (The actual hardware clock ticker)
} TIMER_TypeDef;

// Global instance of Timer 4 (Commonly used for PWM in hardware)
TIMER_TypeDef TIM4 = { .ARR = 10, .CCR = 3, .CNT = 0 };

// --- Core Function: Simulate One Hardware Clock Cycle ---
// This represents what the silicon digital logic does inside the chip on every tick
void Simulate_Timer_PWM_Tick(void) {
    // 1. Hardware Comparator Logic
    if (TIM4.CNT < TIM4.CCR) {
        // Output High (1) during the active part of the duty cycle
        printf("[PWM Output: 1]  (CNT: %2d < CCR: %d)\n", TIM4.CNT, TIM4.CCR);
    } else {
        // Output Low (0) for the remainder of the period
        printf("[PWM Output: 0]  (CNT: %2d >= CCR: %d)\n", TIM4.CNT, TIM4.CCR);
    }

    // 2. Counter Incrementation & Auto-Reload Logic
    TIM4.CNT++;
    if (TIM4.CNT >= TIM4.ARR) {
        printf("--- Period End (ARR Reached). Resetting Counter to 0 ---\n");
        TIM4.CNT = 0; // Hardware wrap around
    }
}

int main() {
    printf("--- Part 1: Running PWM at 30%% Duty Cycle (CCR=3, ARR=10) ---\n");
    // Simulate 1.5 periods to observe the waveform stability
    for (int tick = 0; tick < 15; tick++) {
        Simulate_Timer_PWM_Tick();
    }

    printf("\n--- Part 2: Dynamically Changing Duty Cycle to 70%% (CCR=7) ---\n");
    // This is exactly how you dim an LED or accelerate a motor in firmware!
    TIM4.CCR = 7;
    TIM4.CNT = 0; // Reset counter for clean start
    
    for (int tick = 0; tick < 15; tick++) {
        Simulate_Timer_PWM_Tick();
    }

    return 0;
}