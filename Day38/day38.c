#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// --- Mocking Linux Kernel _IOW / _IOR Command Macro Construction ---
#define IOC_MAGIC 'k' // Unique driver identifier magic character

#define CMD_RESET_HARDWARE   0x01
#define CMD_SET_BAUDRATE     0x02
#define CMD_GET_CHIP_ID      0x03

// Simulated HW Register Bank
typedef struct {
    uint32_t baud_rate;
    uint32_t chip_id;
    bool is_reset;
} Mock_HW_Registers_t;

static Mock_HW_Registers_t g_hw_regs = { .baud_rate = 9600, .chip_id = 0x32F4, .is_reset = false };

// --- Linux Kernel Driver ioctl Handler ---
// Corresponds to: long (*unlocked_ioctl) (struct file *, unsigned int cmd, unsigned long arg)
int my_driver_ioctl(unsigned int cmd, unsigned long arg) {
    printf("[Kernel Driver] ioctl System Call Received. Command ID: 0x%X\n", cmd);

    switch (cmd) {
        case CMD_RESET_HARDWARE:
            printf("  ↳ [HW Control] Executing Hardware Soft Reset...\n");
            g_hw_regs.is_reset = true;
            g_hw_regs.baud_rate = 9600; // Reset to default
            break;

        case CMD_SET_BAUDRATE: {
            uint32_t new_baud = (uint32_t)arg;
            printf("  ↳ [HW Control] Configuring Hardware Baud Rate to: %u bps\n", new_baud);
            g_hw_regs.baud_rate = new_baud;
            break;
        }

        case CMD_GET_CHIP_ID: {
            uint32_t *user_ptr = (uint32_t *)arg;
            if (user_ptr == NULL) return -1; // Fault
            printf("  ↳ [HW Control] Reading Hardware Chip ID (0x%X) to User Space...\n", g_hw_regs.chip_id);
            *user_ptr = g_hw_regs.chip_id; // Safe copy simulation
            break;
        }

        default:
            printf("  ↳ [Kernel Error] Unknown ioctl command 0x%X!\n", cmd);
            return -22; // -EINVAL (Invalid argument)
    }

    return 0; // Success
}

// --- User Space Application Simulation ---
int main() {
    printf("=================================================================\n");
    printf("--- Day 38: Linux Kernel Driver ioctl Interface Simulation ---\n");
    printf("=================================================================\n\n");

    printf("--- Step 1: User App calling ioctl to Set Baudrate to 115200 ---\n");
    my_driver_ioctl(CMD_SET_BAUDRATE, 115200);

    printf("\n--- Step 2: User App calling ioctl to Read Hardware Chip ID ---\n");
    uint32_t rx_chip_id = 0;
    my_driver_ioctl(CMD_GET_CHIP_ID, (unsigned long)&rx_chip_id);
    printf("User Application verified Chip ID: 0x%X\n", rx_chip_id);

    printf("\n--- Step 3: User App calling ioctl to Reset Hardware ---\n");
    my_driver_ioctl(CMD_RESET_HARDWARE, 0);

    printf("\n--- Step 4: Testing Invalid Command ---\n");
    int status = my_driver_ioctl(0xDEAD, 0);
    printf("Driver response for invalid CMD: %d (Invalid Argument)\n", status);

    return 0;
}