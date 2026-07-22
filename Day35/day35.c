#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_NAME "/jetson_telemetry_shm"
#define SHM_SIZE 1024

// Shared Data Structure between Process A (YOLO) and Process B (Thermal Ctrl)
typedef struct {
    int   gpu_temperature;
    int   inference_fps;
    char  system_status[32];
} SharedTelemetry_t;

int main() {
    printf("======================================================\n");
    printf("--- Day 35: Linux POSIX Shared Memory (Zero-Copy) ---\n");
    printf("======================================================\n\n");

    // 1. Create/Open a POSIX Shared Memory Object in /dev/shm
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }
    printf("[IPC System] Shared Memory Object created successfully (FD: %d)\n", shm_fd);

    // 2. Set the size of the memory region
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }

    // 3. Map the shared physical memory directly into current process virtual address space
    SharedTelemetry_t *p_telemetry = (SharedTelemetry_t *)mmap(
        NULL, 
        SHM_SIZE, 
        PROT_READ | PROT_WRITE, 
        MAP_SHARED, 
        shm_fd, 
        0
    );

    if (p_telemetry == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    printf("[IPC System] Memory mapped into Process Virtual Address Space at: %p\n", (void *)p_telemetry);

    // 4. Simulate Process A writing data directly to RAM (Zero-Copy)
    printf("\n[Process A (Writer)] Writing telemetry data straight to Shared RAM...\n");
    p_telemetry->gpu_temperature = 78; // 78 Celsius
    p_telemetry->inference_fps   = 60; // 60 FPS
    strncpy(p_telemetry->system_status, "JETSON_GPU_OPTIMAL", sizeof(p_telemetry->system_status));

    // 5. Simulate Process B reading the exact same RAM address instantly
    printf("[Process B (Reader)] Reading shared RAM data directly without IPC copy:\n");
    printf("  ↳ GPU Thermal Status: %d °C\n", p_telemetry->gpu_temperature);
    printf("  ↳ AI Inference Rate:  %d FPS\n", p_telemetry->inference_fps);
    printf("  ↳ System Status Flag: %s\n", p_telemetry->system_status);

    // 6. Clean up: Unmap memory and unlink object from system
    munmap(p_telemetry, SHM_SIZE);
    close(shm_fd);
    shm_unlink(SHM_NAME); // Removes /dev/shm/jetson_telemetry_shm

    printf("\n[Main System] Shared Memory lifecycle cleaned up safely.\n");
    return 0;
}