#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/epoll.h>

// ============================================================================
// 題目 3: Graceful Shutdown (優雅關機) 與 Signal Handler 安全訊號旗標
// ============================================================================
// ⭐️ 核心安全型態：sig_atomic_t 搭配 volatile 保證單一指令原子寫入與不可快取
static volatile sig_atomic_t g_running = 1;

void sigint_handler(int sig) {
    (void)sig;
    // ⭐️ 核心原則：Signal Handler 內部絕不呼叫 printf/malloc (Async-Signal-Unsafe)
    // 只做最簡單安全的原子性旗標修改！
    g_running = 0;
}

// ============================================================================
// 題目 1 & 2: Epoll O(1) 與 ET/LT 模式說明
// ============================================================================
void explain_epoll_architecture(void) {
    printf("--- 1. Epoll O(1) Architecture (Red-Black Tree + Ready List) ---\n");
    printf("↳ Red-Black Tree in Kernel: Stores all monitored FDs (O(log N) insert/delete).\n");
    printf("↳ Ready Linked List (rdllist): Hardware interrupts automatically append active FDs.\n");
    printf("↳ epoll_wait() checks rdllist in O(1) time without scanning all FDs!\n");

    printf("\n--- 2. Level Triggered (LT) vs Edge Triggered (ET) ---\n");
    printf("↳ LT (Default): Continuously notifies as long as data remains in buffer.\n");
    printf("↳ ET (Edge): Notifies ONLY ONCE on rising edge (0 -> 1 data transition).\n");
    printf("↳ ET MUST be paired with Non-blocking Socket + while(read() != EAGAIN) loop!\n");
    printf("↳ Blocking Socket in ET mode will FREEZE the CPU thread on empty buffer read!\n");
}

// ============================================================================
// 題目 3: Shared Memory 零拷貝概念說明
// ============================================================================
void explain_shared_memory_zerocopy(void) {
    printf("\n--- 3. Shared Memory Zero-Copy Principle ---\n");
    printf("↳ shm_open() + mmap() maps virtual pages of Process A & B to SAME physical RAM.\n");
    printf("↳ Process A writes to memory -> Process B sees it INSTANTLY without kernel memcpy!\n");
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 48: Epoll O(1), Shared Memory & Graceful Shutdown ---\n");
    printf("=================================================================\n\n");

    // 註冊 SIGINT (Ctrl+C) 訊號處理
    signal(SIGINT, sigint_handler);

    explain_epoll_architecture();
    explain_shared_memory_zerocopy();

    printf("\n--- 4. Graceful Shutdown Event Loop Test ---\n");
    printf("↳ System Running... (Press Ctrl+C to trigger Graceful Shutdown)\n");

    int loop_count = 0;
    while (g_running && loop_count < 3) {
        printf("  ↳ Main loop working... iteration %d\n", ++loop_count);
        usleep(200000);
    }

    printf("↳ Loop exited safely! g_running = %d. Closing sockets & freeing resources...\n", g_running);
    printf("↳ Graceful Shutdown Completed Cleanly!\n");

    printf("\n=================================================================\n");
    return 0;
}
