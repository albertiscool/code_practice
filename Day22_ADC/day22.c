#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define ADC_RESOLUTION_BITS 12
#define ADC_MAX_VALUE       4095 // 2^12 - 1
#define V_REF               3.3f // Reference voltage is 3.3V

// --- Core Function: Simulating SAR ADC Hardware Logic ---
// Convert an analog voltage into a 12-bit digital value using Successive Approximation
uint16_t Simulate_SAR_ADC(float input_voltage) {
    // Voltage boundary safety check
    if (input_voltage >= V_REF) return ADC_MAX_VALUE;
    if (input_voltage <= 0.0f)  return 0;

    uint16_t sar_result = 0; // Successive Approximation Register (12-bit)

    printf("[SAR ADC] Starting conversion for input voltage: %.3f V\n", input_voltage);

    // Successive Approximation loop: Bit 11 down to Bit 0 (12 steps)
    for (int i = ADC_RESOLUTION_BITS - 1; i >= 0; i--) {
        // 1. Guessing: Tentatively set the current bit to 1
        sar_result |= (1 << i);

        // 2. DAC Feedback: Convert the tentative digital guess back to voltage to compare
        float current_guess_voltage = ((float)sar_result / ADC_MAX_VALUE) * V_REF;

        // 3. Compare: Hardware comparator judges the voltage
        if (current_guess_voltage > input_voltage) {
            // If our guess is too high, clear this bit back to 0
            sar_result &= ~(1 << i);
            printf("  Bit %2d: Too High -> Set to 0 | Current Approximation: %.3f V\n", i, current_guess_voltage);
        } else {
            // Keep the bit as 1
            printf("  Bit %2d: Safe     -> Keep as 1 | Current Approximation: %.3f V\n", i, current_guess_voltage);
        }
    }

    return sar_result;
}

int main() {
    float test_voltage = 2.5f; // Let's simulate capturing 2.5 Volts from a sensor
    
    printf("--- Part 1: Simulating Hardware SAR Logic ---\n");
    uint16_t digital_code = Simulate_SAR_ADC(test_voltage);
    
    printf("\n--- Part 2: Final Conversion Summary ---\n");
    printf("Analog Input:   %.3f V\n", test_voltage);
    printf("Digital Output: %d (Hex: 0x%03X)\n", digital_code, digital_code);
    
    // Convert back using formula to see the quantization error
    float reconstructed_v = ((float)digital_code / ADC_MAX_VALUE) * V_REF;
    printf("Reconstructed:  %.3f V (Quantization Error: %.5f V)\n", reconstructed_v, test_voltage - reconstructed_v);

    return 0;
}