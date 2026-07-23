#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

// Signal-safe atomic flag to signal system shutdown
static volatile sig_atomic_t g_keep_running = 1;

// --- POSIX Signal Handler Function ---
// Must only use async-signal-safe functions inside (e.g. write(), not printf() in strict rules)
void Signal_Handler(int signum) {
    if (signum == SIGINT) {
        // Soft print for demo purposes
        write(STDOUT_FILENO, "\n[Signal Captured] SIGINT (Ctrl+C) received! Triggering Graceful Shutdown...\n", 76);
        g_keep_running = 0; // Break the main event loop cleanly
    }
}

int main() {
    printf("=========================================================\n");
    printf("--- Day 36: Linux Signal Handling & Graceful Shutdown ---\n");
    printf("=========================================================\n\n");

    // 1. Configure sigaction structure
    struct sigaction sa;
    sa.sa_handler = Signal_Handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // 2. Register SIGINT handler (Catching Ctrl+C)
    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction registration failed");
        return 1;
    }

    printf("[System Init] Signal Handler registered for SIGINT (Signal 2).\n");
    printf("[System Init] Jetson Nano Telemetry & YOLO Pipeline Running...\n");
    printf(">> Press 'Ctrl + C' in your terminal to trigger Graceful Shutdown <<\n\n");

    int loop_counter = 0;
    // 3. Main Event Loop running until signal changes the atomic flag
    while (g_keep_running) {
        loop_counter++;
        printf("  ↳ [Main Loop #%d] Processing AI Inference frames & Tegrastats...\n", loop_counter);
        
        // Simulate work cycle with micro-sleeps to quickly catch signal flag
        for (int i = 0; i < 5 && g_keep_running; i++) {
            usleep(100000); // 100ms
        }
    }

    // 4. Executing Safe Resource Cleanup Phase
    printf("\n--- Graceful Cleanup Phase Started ---\n");
    printf("[Cleanup] Closing camera File Descriptors...\n");
    printf("[Cleanup] Unlinking POSIX Shared Memory (/dev/shm)...\n");
    printf("[Cleanup] Saving final telemetry log file...\n");
    printf("====== System Terminated Cleanly Without Memory Leaks ======\n");

    return 0;
}