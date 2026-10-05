#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

// ============================================================================
// 題目 3: ioctl 巨集與魔術數字模擬 (_IOW / _IOR)
// ============================================================================
#define MYDEV_MAGIC 'k'
#define MYDEV_CMD_RESET   _IO(MYDEV_MAGIC, 1)
#define MYDEV_CMD_SET_VAL _IOW(MYDEV_MAGIC, 2, uint32_t)
#define MYDEV_CMD_GET_VAL _IOR(MYDEV_MAGIC, 3, uint32_t)

// 模擬 Linux _IOW 巨集打包位元 (方向 2-bit, 大小 14-bit, 類型 8-bit, 序號 8-bit)
#define MOCK_IOW(magic, nr, type) ((uint32_t)((magic << 8) | (nr) | (sizeof(type) << 16)))

// ============================================================================
// 題目 1 & 2: Linux Char Device Driver, file_operations & copy_from_user 模擬
// ============================================================================
typedef struct {
    uint32_t major;
    uint32_t minor;
    char kernel_buffer[256];
    uint32_t device_value;
} Mock_CharDevice_t;

static Mock_CharDevice_t g_my_device = {
    .major = 240,
    .minor = 0,
    .device_value = 100
};

// 模擬 copy_from_user 核心防禦搬移
unsigned long mock_copy_from_user(void *to, const void *from, unsigned long n) {
    if (from == NULL) {
        printf("  ↳ [copy_from_user FAIL] Invalid User Pointer (NULL)! Preventing Kernel Panic.\n");
        return n; // 回傳無法複製的 Byte 數量 (n > 0 代表失敗)
    }
    printf("  ↳ [copy_from_user SUCCESS] Safety SMAP check passed & Exception Table ready. Copied %lu bytes.\n", n);
    memcpy(to, from, n);
    return 0; // 回傳 0 代表 100% 成功！
}

// 模擬 unlocked_ioctl 驅動處理
long mock_unlocked_ioctl(uint32_t cmd, unsigned long arg) {
    printf("[Driver ioctl] Received Command: 0x%08X\n", cmd);
    
    if (cmd == MOCK_IOW('k', 2, uint32_t)) {
        g_my_device.device_value = (uint32_t)arg;
        printf("  ↳ [unlocked_ioctl] Updated device_value to: %u (No Big Kernel Lock! Managed by Driver)\n", g_my_device.device_value);
        return 0;
    }
    return -1;
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 49: Linux Char Device Driver & copy_from_user Test ---\n");
    printf("=================================================================\n\n");

    // 1. 主次設備號說明
    printf("--- 1. Device Numbers (Major / Minor) ---\n");
    printf("↳ Major Number: %u (Identifies Driver Type / Category)\n", g_my_device.major);
    printf("↳ Minor Number: %u (Identifies Specific Device Instance / Node)\n", g_my_device.minor);

    // 2. copy_from_user 安全防禦測試
    printf("\n--- 2. copy_from_user vs memcpy Safety Test ---\n");
    const char user_msg[] = "Hello Linux Kernel Driver!";
    unsigned long fail_bytes = mock_copy_from_user(g_my_device.kernel_buffer, user_msg, strlen(user_msg) + 1);
    printf("↳ copy_from_user Return Code: %lu (0 = 100%% Success)\n", fail_bytes);

    // 3. unlocked_ioctl 測試
    printf("\n--- 3. unlocked_ioctl & Macro Command Test ---\n");
    uint32_t cmd = MOCK_IOW('k', 2, uint32_t);
    mock_unlocked_ioctl(cmd, 500);

    printf("\n=================================================================\n");
    return 0;
}
