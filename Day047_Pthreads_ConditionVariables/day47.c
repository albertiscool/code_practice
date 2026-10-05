#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

// ============================================================================
// 題目 2 & 3: Pthreads Condition Variable, While Defense & Broadcast 模擬
// ============================================================================
typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t  cond;
    int g_data_count;
    bool system_emergency_voltage;
} SharedResource_t;

static SharedResource_t g_res = {
    .lock = PTHREAD_MUTEX_INITIALIZER,
    .cond = PTHREAD_COND_INITIALIZER,
    .g_data_count = 0,
    .system_emergency_voltage = false
};

// 模擬 Consumer 執行緒（使用 while 迴圈防禦虛假喚醒）
void *consumer_worker(void *arg) {
    long id = (long)arg;
    pthread_mutex_lock(&g_res.lock);

    // ⭐️ 核心防禦：絕對不使用 if，強制使用 while 防禦虛假喚醒與多 Consumer 搶奪
    while (g_res.g_data_count == 0 && !g_res.system_emergency_voltage) {
        printf("[Consumer #%ld] No data available. Calling pthread_cond_wait (Unlock -> Block -> Lock)...\n", id);
        pthread_cond_wait(&g_res.cond, &g_res.lock);
    }

    if (g_res.system_emergency_voltage) {
        printf("[Consumer #%ld Alert] EMERGENCY VOLTAGE SURGE CAPTURED! Shutting down safely.\n", id);
    } else if (g_res.g_data_count > 0) {
        g_res.g_data_count--;
        printf("[Consumer #%ld Success] Processed 1 data item. Remaining: %d\n", id, g_res.g_data_count);
    }

    pthread_mutex_unlock(&g_res.lock);
    return NULL;
}

// 模擬 Producer 廣播訊號
void trigger_emergency_broadcast(void) {
    pthread_mutex_lock(&g_res.lock);
    g_res.system_emergency_voltage = true;
    printf("\n[Producer System] Emergency Voltage Surge Detected! Triggering pthread_cond_broadcast()...\n");
    
    // ⭐️ 核心 API：使用 broadcast 確保全家大小通通喚醒！
    pthread_cond_broadcast(&g_res.cond);
    pthread_mutex_unlock(&g_res.lock);
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 47: Pthreads, Condition Variables & While Defense Test ---\n");
    printf("=================================================================\n\n");

    // 1. 說明 MMU 記憶體保護
    printf("--- 1. Linux MMU Virtual Memory Isolation ---\n");
    printf("↳ Linux Process uses MMU virtual addresses.\n");
    printf("↳ Invalid pointer access triggers Page Fault -> SIGSEGV (Segmentation fault).\n");
    printf("↳ Only offending Process is terminated; Linux Kernel remains alive and safe!\n");
    printf("↳ Pthreads in same process SHARE Heap, Data segment, and File Descriptors.\n");

    // 2. 測試 Condition Variable Broadcast
    printf("\n--- 2. Condition Variable Broadcast Simulation ---\n");
    pthread_t c1, c2;
    pthread_create(&c1, NULL, consumer_worker, (void*)1);
    pthread_create(&c2, NULL, consumer_worker, (void*)2);

    usleep(100000); // 確保 Consumer 都進入 wait
    trigger_emergency_broadcast();

    pthread_join(c1, NULL);
    pthread_join(c2, NULL);

    printf("\n=================================================================\n");
    return 0;
}
