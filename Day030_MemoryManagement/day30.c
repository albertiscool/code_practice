#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define HEAP_SIZE 128

// Simulated physical Heap memory pool
static uint8_t ucHeap[HEAP_SIZE];

// Block header to keep track of memory chunks
typedef struct A_BLOCK_LINK {
    struct A_BLOCK_LINK *pxNextFreeBlock; // Pointer to the next free block
    size_t xBlockSize;                    // Size of this block (including header)
} BlockLink_t;

// Define the start and end markers of our free list
static BlockLink_t xStart, *pxEnd = NULL;

// --- Core Function: Initialize Mock Heap (heap_4 style) ---
void vPortInitialiseHeap(void) {
    BlockLink_t *pxFirstFreeBlock;

    // Set up xStart to point to the first free block
    xStart.pxNextFreeBlock = (BlockLink_t *)ucHeap;
    xStart.xBlockSize = 0;

    // The entire heap is initially one big free block
    pxFirstFreeBlock = (BlockLink_t *)ucHeap;
    pxFirstFreeBlock->xBlockSize = HEAP_SIZE;
    pxFirstFreeBlock->pxNextFreeBlock = pxEnd; // Points to NULL (end of list)
}

// --- Core Function: Free Memory with Coalescing (heap_4 style) ---
void vPortFree(void *pv) {
    if (pv == NULL) return;

    // Move back to get the block header address
    uint8_t *puc = (uint8_t *)pv;
    puc -= sizeof(BlockLink_t);
    BlockLink_t *pxBlockToFree = (BlockLink_t *)puc;

    printf("\n[vPortFree] Freeing block of size %zu bytes at address %p\n", 
           pxBlockToFree->xBlockSize, (void *)pxBlockToFree);

    // Iterate through the free list to find where to insert this block back (sorted by address)
    BlockLink_t *pxIterator;
    for (pxIterator = &xStart; pxIterator->pxNextFreeBlock < pxBlockToFree && pxIterator->pxNextFreeBlock != pxEnd; pxIterator = pxIterator->pxNextFreeBlock) {
        // Just searching...
    }

    // Insert the freed block back into the free list
    pxBlockToFree->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
    pxIterator->pxNextFreeBlock = pxBlockToFree;

    // --- COALESCING LOGIC (Merging adjacent blocks) ---
    // 1. Merge with the NEXT block if they are physically adjacent
    if ((uint8_t *)pxBlockToFree + pxBlockToFree->xBlockSize == (uint8_t *)pxBlockToFree->pxNextFreeBlock) {
        printf("  ↳ Merging with the NEXT adjacent block!\n");
        pxBlockToFree->xBlockSize += pxBlockToFree->pxNextFreeBlock->xBlockSize;
        pxBlockToFree->pxNextFreeBlock = pxBlockToFree->pxNextFreeBlock->pxNextFreeBlock;
    }

    // 2. Merge with the PREVIOUS block if they are physically adjacent
    if ((uint8_t *)pxIterator + pxIterator->xBlockSize == (uint8_t *)pxBlockToFree) {
        printf("  ↳ Merging with the PREVIOUS adjacent block!\n");
        pxIterator->xBlockSize += pxBlockToFree->xBlockSize;
        pxIterator->pxNextFreeBlock = pxBlockToFree->pxNextFreeBlock;
    }
}

// Helper to print current free blocks
void prvPrintFreeList(void) {
    BlockLink_t *pxIterator = xStart.pxNextFreeBlock;
    printf("[Free List Map]: ");
    while (pxIterator != pxEnd) {
        printf("[Addr: %p, Size: %zu] -> ", (void *)pxIterator, pxIterator->xBlockSize);
        pxIterator = pxIterator->pxNextFreeBlock;
    }
    printf("NULL\n");
}

int main() {
    printf("--- FreeRTOS Heap_4 Memory Coalescing Simulation ---\n\n");
    vPortInitialiseHeap();
    prvPrintFreeList();

    // Let's simulate allocating two virtual chunks (we'll manually construct the block offsets)
    // Block A is at the beginning, Block B is physically right after Block A
    BlockLink_t *blockA = (BlockLink_t *)ucHeap;
    blockA->xBlockSize = 48; // Allocate 48 bytes (including header)
    
    BlockLink_t *blockB = (BlockLink_t *)((uint8_t *)blockA + 48);
    blockB->xBlockSize = 48; // Allocate 48 bytes (including header)

    // The remaining memory is Block C (size = 128 - 48 - 48 = 32 bytes)
    BlockLink_t *blockC = (BlockLink_t *)((uint8_t *)blockB + 48);
    blockC->xBlockSize = 32;
    blockC->pxNextFreeBlock = pxEnd;

    // Set our free list to mimic that only Block C is currently free
    xStart.pxNextFreeBlock = blockC;
    printf("\n--- Initial State (Block A & B are occupied, only C is free) ---\n");
    prvPrintFreeList();

    // Now, free Block B first. It shouldn't merge with Block A (occupied), but might merge with Block C (free)
    vPortFree((uint8_t *)blockB + sizeof(BlockLink_t));
    prvPrintFreeList();

    // Next, free Block A. Since Block B was already freed and merged, Block A should merge with everything!
    vPortFree((uint8_t *)blockA + sizeof(BlockLink_t));
    prvPrintFreeList();

    return 0;
}