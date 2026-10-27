#include <stdio.h>
#include <stddef.h>
#include <string.h>

/*
 * 【群聯底層手撕題】記憶體重疊防禦手寫 my_memmove
 *
 * 題目背景與情境：
 * 在 SSD 控制器、DMA 傳輸與 Buffer 管理中，資料來源 (src) 與目標 (dest) 記憶體區塊
 * 可能會發生重疊（Memory Overlap）。
 * 標準庫的 memcpy 遇到重疊記憶體會引發未定義行為（Undefined Behavior）導致資料覆蓋錯亂。
 * 請手撕一個工業級安全的 my_memmove 函式。
 *
 * 函式原型：
 * void* my_memmove(void *dest, const void *src, size_t n);
 *
 * 限制與要求：
 * 1. 嚴禁使用 <string.h> 中的任何字串或記憶體拷貝函式（如 memcpy, memmove, strcpy 等）。
 * 2. 空間複雜度必須為 O(1)，禁止使用 malloc 分配暫存陣列。
 * 3. 需精準判斷 dest 與 src 的相對位址：
 *    - 當重疊時，必須保證源資料在被覆蓋之前就已被拷貝完畢。
 * 4. 回傳 dest 原指標。
 */

void* my_memmove(void *dest, const void *src, size_t n) {
    // 邊界防禦：指標為空、長度為 0、或原地拷貝直接返回
    if (dest == NULL || src == NULL || n == 0 || dest == src) {
        return dest;
    }

    // 轉型為單字節 (Byte) 指標，以進行安全的指針位移與解引用
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    // 【關鍵判斷】：當 dest 位於 src 的後方且發生重疊時，必須從尾巴往頭部拷貝（反向拷貝）
    if (d > s && d < s + n) {
        for (size_t i = n; i > 0; i--) {
            d[i - 1] = s[i - 1]; // 避免 off-by-one 越界
        }
    } else {
        // 其餘情況（dest 位於 src 前方，或兩區塊完全無重疊）：正向拷貝即可
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    }

    return dest;
}

/*
================================================================================
【原始白板代碼保留與盲點詳細剖析】
================================================================================
void* my_memmove_Original(void *dest, const void *src, size_t n) {
    // ❌ 盲點 1：條件不足！當 dest < src 時，src + n > dest 依然成立，但此時應為正向拷貝。
    // 必須是 (dest > src && dest < src + n) 才需要反向拷貝。
    if(src + n > dest)
    {
        // ❌ 盲點 2（語法錯誤）：for(for ... 多打了一個 for。
        // ❌ 盲點 3（索引越界）：*(dest + i) 當 i = n 時越界存取了 dest[n]（有效範圍為 0 ~ n-1）。
        // ❌ 盲點 4（語法錯誤）：void* 不能直接加整數或用 * 解引用，需先轉型為 char* / unsigned char*。
        for(for int i = n ; i > 0; i--)
        {
            *(dest + i) = *(src + i);
        }
    }
    else
    {
        for(int i = 0; i < n; i++)
        {
            *(dest + i) = *(src + i); // ❌ void* 解引用編譯報錯
        }
    }
    // ❌ 盲點 5：漏了 return dest;
}
================================================================================
*/

int main() {
    // Test 1: 非重疊拷貝
    printf("=== Test 1: Non-overlapping copy ===\n");
    char src1[] = "Hello, Phison!";
    char dst1[30] = {0};
    my_memmove(dst1, src1, strlen(src1) + 1);
    printf("Result:   \"%s\"\n", dst1);
    printf("Expected: \"Hello, Phison!\"\n");

    // Test 2: 重疊拷貝 (dest 在 src 後方: dest > src)
    // "1234567890" -> 將 "12345" 拷貝到 index 2 ("1212345890")
    printf("\n=== Test 2: Overlapping (dest > src) ===\n");
    char str2[] = "1234567890";
    my_memmove(str2 + 2, str2, 5);
    printf("Result:   \"%s\"\n", str2);
    printf("Expected: \"1212345890\"\n");

    // Test 3: 重疊拷貝 (dest 在 src 前方: dest < src)
    // "1234567890" -> 將 "34567" 拷貝到 index 0 ("3456767890")
    printf("\n=== Test 3: Overlapping (dest < src) ===\n");
    char str3[] = "1234567890";
    my_memmove(str3, str3 + 2, 5);
    printf("Result:   \"%s\"\n", str3);
    printf("Expected: \"3456767890\"\n");

    // Test 4: dest == src (原樣不變)
    printf("\n=== Test 4: dest == src ===\n");
    char str4[] = "NoChange";
    my_memmove(str4, str4, strlen(str4) + 1);
    printf("Result:   \"%s\"\n", str4);
    printf("Expected: \"NoChange\"\n");

    return 0;
}
