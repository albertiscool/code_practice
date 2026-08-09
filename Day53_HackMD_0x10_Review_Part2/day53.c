#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// HackMD 0x10 Q4: 無窮迴圈與低功耗 WFI
// ============================================================================
void demo_q4_infinite_loops(void) {
    printf("--- HackMD Q4: Infinite Loops & Low-Power WFI ---\n");
    printf("↳ 3 Syntaxes: while(1), for(;;), goto Loop.\n");
    printf("↳ for(;;) is preferred by linters to avoid 'constant conditional' warnings.\n");
    printf("↳ Low-power MCU uses __WFI() (Wait For Interrupt) in loop to sleep CPU.\n");
}

// ============================================================================
// HackMD 0x10 Q5: 複雜型態宣告與指標對決 (a ~ h)
// ============================================================================
int dummy_func(int x) { return x * 2; }

void demo_q5_complex_declarations(void) {
    int a_var = 10;
    int *a_ptr = &a_var;                  // b) Pointer to int
    int **a_pptr = &a_ptr;                // c) Pointer to pointer to int
    int a_arr[10] = {0};                  // d) Array of 10 ints
    int *a_ptr_arr[10] = { &a_var };       // e) Array of 10 pointers to int
    int (*a_arr_ptr)[10] = &a_arr;        // f) Pointer to array of 10 ints
    int (*a_func_ptr)(int) = dummy_func;  // g) Pointer to function(int) -> int
    int (*a_func_arr[10])(int) = { dummy_func }; // h) Array of 10 pointers to function(int) -> int

    printf("\n--- HackMD Q5: Complex Declarations (a ~ h) ---\n");
    printf("↳ e) int *a[10]: Array of 10 pointers to int.\n");
    printf("↳ f) int (*a)[10]: Pointer to array of 10 ints.\n");
    printf("↳ g) int (*a)(int): Function pointer -> %d\n", a_func_ptr(5));
    printf("↳ h) int (*a[10])(int): Array of 10 function pointers -> %d\n", a_func_arr[0](10));
}

// ============================================================================
// HackMD 0x10 Q6: static 關鍵字 3 大作用
// ============================================================================
static int g_file_private_var = 100; // 1. 隱藏：僅限本 .c 存取

void demo_q6_static_lifetime(void) {
    static int counter = 0; // 2. 延長生命週期：放在 .data/.bss 段，跨呼叫保留數值
    counter++;
    printf("↳ static counter = %d (Persistent lifetime!)\n", counter);
}

// ============================================================================
// Main 測試主程式
// ============================================================================
int main() {
    printf("=================================================================\n");
    printf("--- Day 53: HackMD 0x10 Firmware Interview Review Part 2 ---\n");
    printf("=================================================================\n\n");

    demo_q4_infinite_loops();
    demo_q5_complex_declarations();
    
    printf("\n--- HackMD Q6: static Keyword Lifetime Demo ---\n");
    demo_q6_static_lifetime();
    demo_q6_static_lifetime();
    demo_q6_static_lifetime();

    printf("\n=================================================================\n");
    return 0;
}
