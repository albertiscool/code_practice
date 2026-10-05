#include <stdio.h>

// 1. Define two standard functions matching the profile: void function(void)
void turn_on_led(void) {
    printf("[App Layer] Callback executed: LED turned ON!\n");
}

void play_buzzer_sound(void) {
    printf("[App Layer] Callback executed: Buzzer BEEP!\n");
}

// 2. Mock Driver Layer: Struct that holds a function pointer
typedef struct {
    void (*on_click_callback)(void); // Function pointer member
} Button_Driver_t;

// Simulated Hardware ISR (Interrupt Service Routine) or Event Trigger
void simulate_hardware_button_press(Button_Driver_t *button) {
    printf("[Driver Layer] Hardware Interrupt: Button pressed!\n");
    
    // Check if a callback is registered before calling it (Safety first!)
    if (button->on_click_callback != NULL) {
        button->on_click_callback(); // Trigger the callback via pointer
    } else {
        printf("[Driver Layer] No callback registered.\n");
    }
}

int main() {
    printf("--- Part 1: Basic Function Pointer ---\n");
    // Declare a function pointer and point it to turn_on_led
    void (*simple_fp)(void) = turn_on_led;
    printf("Address of turn_on_led function: %p\n", (void*)turn_on_led);
    printf("Address stored in simple_fp:     %p\n", (void*)simple_fp);
    simple_fp(); // Invoke function via pointer

    printf("\n--- Part 2: Mock Driver Callback Architecture ---\n");
    Button_Driver_t my_button = { .on_click_callback = NULL };

    // Test case A: Press button without registering anything
    simulate_hardware_button_press(&my_button);

    // Test case B: Register LED function to the button
    printf("\n-> Registering LED function...\n");
    my_button.on_click_callback = turn_on_led;
    simulate_hardware_button_press(&my_button);

    // Test case C: Dynamically switch callback to Buzzer function
    printf("\n-> Switching to Buzzer function...\n");
    my_button.on_click_callback = play_buzzer_sound;
    simulate_hardware_button_press(&my_button);

    return 0;
}