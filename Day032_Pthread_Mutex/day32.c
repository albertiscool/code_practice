#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Global shared resource
int g_shared_resource_counter = 0;

// POSIX Mutex to protect our shared resource
pthread_mutex_t v_resource_mutex;

// --- Thread Task 1: Simulating High-Rate Telemetry Write ---
void* TelemetryThreadTask(void* arg) {
    printf("[Thread: Telemetry] Safely updating system telemetry counter...\n");
    
    for (int i = 0; i < 3; i++) {
        // 1. Acquire the POSIX lock (Blocks if already locked)
        pthread_mutex_lock(&v_resource_mutex);
        
        // Critical Section: Safely modify global variable
        g_shared_resource_counter++;
        printf("  ↳ Telemetry incremented counter to: %d\n", g_shared_resource_counter);
        
        // 2. Release the lock
        pthread_mutex_unlock(&v_resource_mutex);
        
        usleep(100000); // Sleep for 100ms (Linux standard microsecond sleep)
    }
    return NULL;
}

// --- Thread Task 2: Simulating YOLO Inference State Update ---
void* InferenceThreadTask(void* arg) {
    printf("[Thread: Inference] Safely updating inference cycle counter...\n");
    
    for (int i = 0; i < 3; i++) {
        pthread_mutex_lock(&v_resource_mutex);
        
        g_shared_resource_counter++;
        printf("  ↳ Inference incremented counter to: %d\n", g_shared_resource_counter);
        
        pthread_mutex_unlock(&v_resource_mutex);
        
        usleep(120000); // Sleep for 120ms
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    printf("====================================================\n");
    printf("--- Day 32: Linux Pthread & Mutex Native Training ---\n");
    printf("====================================================\n\n");

    // 1. Initialize the native Linux mutex
    if (pthread_mutex_init(&v_resource_mutex, NULL) != 0) {
        printf("Mutex initialization failed!\n");
        return 1;
    }

    // 2. Spawn two parallel running threads inside Linux Kernel
    pthread_create(&thread1, NULL, TelemetryThreadTask, NULL);
    pthread_create(&thread2, NULL, InferenceThreadTask, NULL);

    // 3. Wait for both threads to finish execution (Blocking Join)
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // 4. Clean up resources
    pthread_mutex_destroy(&v_resource_mutex);

    printf("\n[Main Program] Both threads closed. Final Counter Value: %d\n", g_shared_resource_counter);
    printf("====================================================\n");
    return 0;
}