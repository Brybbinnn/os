#include "malloc.h"

#include <stdio.h>
#include <assert.h>

#include "testlib.h"
#include "malloc.h"
#include <stdio.h>

/*
 * This is the heap you should use.
 * The heap does not grow.
 */
uint8_t __attribute__ ((aligned(HEADER_SIZE))) _heapData[HEAP_SIZE];

/*
 * This should point to the first free block in memory.
 */
Block *_firstFreeBlock;

/*
 * Initializes the memory block. You don't need to change this.
 */
void initAllocator()
{
	_firstFreeBlock = (Block*)&_heapData[0];
	_firstFreeBlock->next = NULL;
	_firstFreeBlock->size = HEAP_SIZE;
}

/*
 * Gets the next block that should start after the current one.
 */
static Block *_getNextBlockBySize(const Block *current)
{
	static const Block *end = (Block*)&_heapData[HEAP_SIZE];
	Block *next = (Block*)&current->data[current->size - HEADER_SIZE];

	assert(next <= end);
	return (next == end) ? NULL : next;
}

/*
 * Dumps the allocator. You should not need to modify this.
 */
void dumpAllocator()
{
	Block *current;
	/* Note: This sample code prints addresses relative to the beginning of the heap */

	/* Part a: all blocks, using size, and starting at the beginning of the heap */
	printf("All blocks:\n");
	current = (Block*)&_heapData[0];
	while (current) {
		printf("  Block starting at %" PRIuPTR ", size %" PRIu64 " (%s)\n",
			((uintptr_t)(void*)current - (uintptr_t)(void*)&_heapData[0]),
			current->size,
			current->magic == ALLOCATED_BLOCK_MAGIC ? "in use" : "free" );

		current = _getNextBlockBySize(current);
	}

	/* Part b: free blocks, using next pointer, and starting at _firstFreeBlock */
	printf("Free block list:\n");
	current = _firstFreeBlock;
	while (current) {
		printf("  Free block starting at %" PRIuPTR ", size %" PRIu64 "\n",
			((uintptr_t)(void*)current - (uintptr_t)(void*)&_heapData[0]),
			current->size);

		current = current->next;
	}
}

/*
 * Round the integer up to the block header size (16 Bytes).
 */
uint64_t roundUp(uint64_t n)
{

	//TODO: Implement
	if (n % 16 == 0) {
		return n;
	} else {
		n = ((n / 16) + 1) * 16;
	}

	return n;
}

/*
 * Internal helper function for making a new allocation
 * (not needed for any tests, but a suggested structure to simplify your life)
 * This function should basically do the last two items of the assignment description
 * of part b (my_malloc).
 * - block is the (currently free) block that you want to use for your new allocation
 * - new_size is the total size for the new allocation (size requested in the
 *   my_malloc call plus header size)
 * - update_next is a pointer to the next pointer pointing to block. This is
 *   the pointer you need to update for removing block from the free-list/
 *   replace block with a new free block starting somewhere within block
 */
static void * __attribute__ ((unused)) allocate_block(Block **update_next, Block *block, uint64_t new_size) {

    uint64_t remainingSize = block->size - new_size;
    if (remainingSize > sizeof(Block)) { // Ensure there's enough space for a new block header
        // Create a new free block with the remaining size
        Block *block2Allocate = (Block *)((char *)block + new_size);
        block2Allocate->size = remainingSize;
        block2Allocate->next = block->next;

        // Update the free list
        *update_next = block2Allocate;
        
        // Adjust the size of the block being allocated
        block->size = new_size;
    } else {
        // If we can't split the block, just remove it from the free list
        *update_next = block->next;
    }

    block->magic = ALLOCATED_BLOCK_MAGIC;

    return *block->data; // Return a pointer to the data portion
}


void *my_malloc(uint64_t size)
{
	// Suggested approach: Search for a free block, then call allocate_block with that block
 	// (and suitable values for update_next and new_size)
	// This is not mandatory, what counts in the and is that my_malloc does the right thing.

	// roundUp size and add header size
	uint64_t totalSize = roundUp(size) + 16;

    Block **prevNext = &_firstFreeBlock;
    Block *current = _firstFreeBlock;

    while (current) {
        if (current->size >= totalSize) {
            return allocate_block(prevNext, current, totalSize);
        }
        prevNext = &current->next;
        current = current->next;
    }

    // No suitable block found
    return NULL;
}

/*
 * Internal helper function to merge two blocks.
 * (not needed for any tests, but a suggested structure to simplify your life)
 * Suggested functionality (to be used in my_free):
 * if block2 follows directly after block1, then merge these two (free) blocks
 * i.e. update block1 such that it becomes one larger free block
 */
static void __attribute__ ((unused)) merge_blocks(Block *block1, Block *block2)
{
	(void)block1;
	(void)block2;
	// TODO: Implement
}


void my_free(void *address)
{
	// TODO: Implement

	// !!
	// MUNA að láta _firstfreeblock verða pointer á address ef address er á undan gamla _firstfreeblock!!

	if (address == NULL) {
		return;
	} else {
		Block *hptr = (Block *)ptr - 1;

	}

}


// int main()
// {
// 	test_start("malloc.c");
// 	initAllocator();

// 	void *a = my_malloc(2016);
// 	my_malloc(3);
// 	my_malloc(28372);
// 	void *b = my_malloc(16);
// 	void *c = my_malloc(5);
// 	my_malloc(2112);

// 	dumpAllocator();

// 	my_free(b);
// 	my_free(c);
// 	my_free(a);
// 	dumpAllocator();
// 	my_malloc(16746540);
// 	my_malloc(1);
// 	my_malloc(1);
// 	my_malloc(1);
// 	my_malloc(1);
// 	dumpAllocator();
// 	my_malloc(16775168);
// 	my_malloc(1);
// 	dumpAllocator();
	
// 	printf("\nNote: This test always passes. It does not do any checks.\n It just prints the memory layout after some operations\n");
// 	return test_end();
// }
