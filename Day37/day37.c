#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define KERNEL_BUFFER_SIZE 64

// --- Mocking Linux Kernel Space Memory & Driver State ---
typedef struct {
    uint8_t kbuf[KERNEL_BUFFER_SIZE]; // Physical device buffer inside Kernel Space
    size_t data_size;
    bool is_opened;
} Mock_CharDevice_t;

static Mock_CharDevice_t g_my_dev = { .data_size = 0, .is_opened = false };

// --- Mocking Kernel Security APIs ---
// In real Linux Driver, copy_from_user checks virtual memory page tables safely
int mock_copy_from_user(void *to_kernel, const void *from_user, size_t count) {
    if (from_user == NULL || to_kernel == NULL) return -1; // Fault
    memcpy(to_kernel, from_user, count);
    return 0; // Success (0 bytes uncopied)
}

int mock_copy_to_user(void *to_user, const void *from_kernel, size_t count) {
    if (to_user == NULL || from_kernel == NULL) return -1; // Fault
    memcpy(to_user, from_kernel, count);
    return 0; // Success
}

// --- Linux Kernel Driver file_operations Hook Functions ---

// Called when user app executes: open("/dev/my_sensor", O_RDWR);
int my_driver_open(void) {
    if (g_my_dev.is_opened) {
        printf("[Kernel Driver] Device /dev/my_sensor is BUSY!\n");
        return -1; // Device busy
    }
    g_my_dev.is_opened = true;
    printf("[Kernel Driver] Device /dev/my_sensor OPENED successfully.\n");
    return 0;
}

// Called when user app executes: write(fd, buffer, len);
ssize_t my_driver_write(const char *user_buffer, size_t count) {
    if (!g_my_dev.is_opened) return -1;
    if (count > KERNEL_BUFFER_SIZE) count = KERNEL_BUFFER_SIZE;

    printf("[Kernel Driver] System Call: write() triggered. Copying from User Space...\n");
    
    // Safely copy data from User Space to Kernel Space
    if (mock_copy_from_user(g_my_dev.kbuf, user_buffer, count) != 0) {
        printf("  ↳ [Kernel Error] Segmentation Fault in User Pointer!\n");
        return -1;
    }

    g_my_dev.data_size = count;
    printf("  ↳ [Kernel Space Success] Saved %zu bytes into device hardware buffer.\n", count);
    return count;
}

// Called when user app executes: read(fd, buffer, len);
ssize_t my_driver_read(char *user_buffer, size_t count) {
    if (!g_my_dev.is_opened) return -1;
    if (count > g_my_dev.data_size) count = g_my_dev.data_size;

    printf("[Kernel Driver] System Call: read() triggered. Copying to User Space...\n");

    // Safely copy data from Kernel Space to User Space
    if (mock_copy_to_user(user_buffer, g_my_dev.kbuf, count) != 0) {
        printf("  ↳ [Kernel Error] Bad User Address!\n");
        return -1;
    }

    printf("  ↳ [Kernel Space Success] Transferred %zu bytes to User Space.\n", count);
    return count;
}

// Called when user app executes: close(fd);
int my_driver_release(void) {
    g_my_dev.is_opened = false;
    printf("[Kernel Driver] Device /dev/my_sensor RELEASED.\n");
    return 0;
}

// --- User Space Application Simulation ---
int main() {
    printf("=============================================================\n");
    printf("--- Day 37: Linux Kernel Char Device Driver Architecture ---\n");
    printf("=============================================================\n\n");

    // User Space buffer
    char app_tx_buf[] = "HW_REGISTER_CMD_0x3F";
    char app_rx_buf[64] = {0};

    printf("--- Step 1: User Application calling open() ---\n");
    my_driver_open();

    printf("\n--- Step 2: User Application calling write() ---\n");
    my_driver_write(app_tx_buf, strlen(app_tx_buf));

    printf("\n--- Step 3: User Application calling read() ---\n");
    my_driver_read(app_rx_buf, strlen(app_tx_buf));

    printf("\n--- Step 4: Verifying Data in User Space ---\n");
    printf("User Application Buffer Received: \"%s\"\n", app_rx_buf);

    printf("\n--- Step 5: User Application calling close() ---\n");
    my_driver_release();

    return 0;
}