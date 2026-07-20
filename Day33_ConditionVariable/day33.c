#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

// Shared global variables
int g_system_voltage_status = 0;
const int VOLTAGE_THRESHOLD = 3;

// POSIX Synchronization Objects
pthread_mutex_t v_status_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  v_status_cond  = PTHREAD_COND_INITIALIZER;

// --- Thread 1: The Producer (Simulating Sensor Telemetry) ---
void* SensorProducerTask(void* arg) {
    for (int i = 1; i <= 4; i++) {
        usleep(100000); // Simulate 100ms hardware data collection delay
        
        pthread_mutex_lock(&v_status_mutex);
        
        g_system_voltage_status = i;
        printf("[Producer] Voltage updated to: %d V\n", g_system_voltage_status);
        
        // Check if the specific condition is met
        if (g_system_voltage_status >= VOLTAGE_THRESHOLD) {
            printf("[Producer] Threshold reached! Sending signal to wake up consumer...\n");
            // Signal and wake up ONE thread waiting on this condition variable
            pthread_cond_signal(&v_status_cond);
        }
        
        pthread_mutex_unlock(&v_status_mutex);
    }
    return NULL;
}

// --- Thread 2: The Consumer (Simulating Safety Controller) ---
void* SafetyConsumerTask(void* arg) {
    pthread_mutex_lock(&v_status_mutex);
    
    // Whle loop guard against spurious wakeups (Standard Linux multi-thread safety rule)
    while (g_system_voltage_status < VOLTAGE_THRESHOLD) {
        printf("[Consumer] Voltage (%d V) < Threshold (%d V). Going to sleep, releasing CPU...\n", 
               g_system_voltage_status, VOLTAGE_THRESHOLD);
        
        /* 
         * Magic Line: pthread_cond_wait does 3 actions ATOMICALLY:
         * 1. Puts this thread to sleep.
         * 2. Unlocks v_status_mutex so the producer can modify the data.
         * 3. (Upon waking up) Automatically re-locks v_status_mutex before returning!
         */
        pthread_cond_wait(&v_status_cond, &v_status_mutex);
        
        printf("[Consumer] Woken up! Checking condition again...\n");
    }
    
    // Critical action executed when condition is successfully met
    printf("[Consumer] !!! ALERT !!! High voltage confirmed: %d V. Executing emergency throttling.\n", g_system_voltage_status);
    
    pthread_mutex_unlock(&v_status_mutex);
    return NULL;
}

int main() {
    pthread_t producer_tid, consumer_tid;

    printf("--- Day 33: Linux Condition Variable & Production Model ---\n\n");

    // Spawn the Consumer first to show it waiting gracefully
    pthread_create(&consumer_tid, NULL, SafetyConsumerTask, NULL);
    usleep(10000); // Brief pause to guarantee consumer locks first for clean stdout logs
    
    pthread_create(&producer_tid, NULL, SensorProducerTask, NULL);

    // Dynamic sync catch
    pthread_join(producer_tid, NULL);
    pthread_join(consumer_tid, NULL);

    printf("\n[Main System] Closed safely. End of Day 33.\n");
    return 0;
}