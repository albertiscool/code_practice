#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define PAGE_SIZE 4096 // Standard Linux Memory Page Size (4KB)

// Simulated Kernel Physical Memory Page allocated by Driver (e.g. kmalloc)
typedef struct {
    uint8_t physical_ram_page[PAGE_SIZE];
    bool is_mapped;
} Mock_Kernel_Memory_t;

static Mock_Kernel_Memory_t g_driver_kmalloc_area = {
    .is_mapped = false
};

// --- Linux Kernel Driver mmap Hook Function ---
// Corresponds to: int (*mmap) (struct file *filp, struct vm_area_struct *vma)
void* my_driver_mmap(void) {
    printf("[Kernel Driver: mmap] System Call mmap() Triggered!\n");
    printf("  ↳ Mapping Kernel Physical Address (%p) to User Virtual Address Space...\n", 
           (void*)g_driver_kmalloc_area.physical_ram_page);
    
    g_driver_kmalloc_area.is_mapped = true;

    // Returns the pointer directly pointing to the exact same RAM space
    return (void*)g_driver_kmalloc_area.physical_ram_page;
}

int main() {
    printf("=================================================================\n");
    printf("--- Day 40: Linux Driver mmap Zero-Copy Memory Simulation ---\n");
    printf("=================================================================\n\n");

    // 1. Pre-fill Kernel memory with simulated hardware sensor data
    const char *hw_data = "DMA_ADC_SAMPLES_BUFFER_RAW_0xFA";
    memcpy(g_driver_kmalloc_area.physical_ram_page, hw_data, strlen(hw_data));

    printf("--- Step 1: User Application Requesting mmap() ---\n");
    // User gets a virtual pointer mapped directly to Kernel memory
    char *user_mapped_ptr = (char *)my_driver_mmap();

    printf("\n--- Step 2: User Reading Data via Mapped Pointer (Zero-Copy) ---\n");
    printf("  ↳ User App Read Result: \"%s\"\n", user_mapped_ptr);

    printf("\n--- Step 3: User Writing Data directly into Kernel RAM ---\n");
    const char *app_write = "USER_CONFIGURED_REGISTER_SET";
    strcpy(user_mapped_ptr, app_write);

    printf("\n--- Step 4: Verifying Data inside Kernel Driver Space ---\n");
    printf("  ↳ Kernel Buffer Status: \"%s\"\n", (char*)g_driver_kmalloc_area.physical_ram_page);

    printf("\n[Main System] Day 40 Zero-Copy Memory Mapping Verified Successfully.\n");
    return 0;
}