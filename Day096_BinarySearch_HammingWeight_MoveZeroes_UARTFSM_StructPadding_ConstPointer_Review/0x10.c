#include <stdio.h>

// ============================================================================
// 0x10 題 1【結構體記憶體對齊與 Padding 計算 (Struct Alignment)】
// ============================================================================
typedef struct {
    char a;   // 1 Byte  + 3 Bytes Padding
    int b;    // 4 Bytes
    short c;  // 2 Bytes + 2 Bytes Tail Padding
} StructA;   // 總計: 1 + 3 + 4 + 2 + 2 = 12 Bytes

typedef struct {
    int b;    // 4 Bytes
    short c;  // 2 Bytes
    char a;   // 1 Byte  + 1 Byte Tail Padding
} StructB;   // 總計: 4 + 2 + 1 + 1 = 8 Bytes

// ============================================================================
// 0x10 題 2【指標常數與常數指標 (Const Pointers)】
// ============================================================================
void test_const_pointers(void) {
    int x = 10;
    int y = 20;

    // 1. const int *p1 (指向常數的指標：內容不可改，指標位址可改)
    const int *p1 = &x;
    // *p1 = 15; // ❌ 編譯錯誤：assignment of read-only location '*p1'
    p1 = &y;     // ✅ 合法：可以變更指向

    // 2. int * const p2 (常數指標：指標位址不可改，內容可改)
    int * const p2 = &x;
    *p2 = 30;    // ✅ 合法：可以修改內容
    // p2 = &y;  // ❌ 編譯錯誤：assignment of read-only variable 'p2'

    // 3. const int * const p3 (雙重鎖死：位址與內容皆不可改)
    const int * const p3 = &x;
    // *p3 = 40; // ❌ 編譯錯誤
    // p3 = &y;  // ❌ 編譯錯誤
    (void)p1;
    (void)p2;
    (void)p3;
}

int main(void) {
    printf("=== Day 96: 0x10 驗證 ===\n");
    printf("sizeof(StructA) = %zu Bytes (預期: 12 Bytes)\n", sizeof(StructA));
    printf("sizeof(StructB) = %zu Bytes (預期: 8 Bytes, 省下 33%% RAM!)\n", sizeof(StructB));

    test_const_pointers();
    printf("Const Pointers 驗證完成！\n");
    return 0;
}
