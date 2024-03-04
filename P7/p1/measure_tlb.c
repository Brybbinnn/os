#define _POSIX_C_SOURCE 199309L

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "measure_tlb.h"

#define PAGESIZE 4096

int64_t measureFunction( int(*function)(void *), void *arg ) {
	// TODO: Implement! (Part P7.1a)
	(void)function;
	(void)arg;

// 	Invokes the function referenced by function, passing arg as parameter;
// before and after the invocation, uses the API function clock gettime (see man
// page) to obtain a high-precision time stamp; and finally
// if function returns an error code (value < 0), returns -1;
// otherwise, returns the elapsed time (difference between the two time stamps) in
// nanoseconds.

	// Gibb Code, Not Tested
	struct timespec start, end;

    // Get start time
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Invoke the function
    int result = function(arg);

    // Get end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate elapsed time in nanoseconds
    int64_t elapsed_ns = (int64_t)(end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    if (result < 0) {
        return -1;
    } else {
        return elapsed_ns;
    }
}

int accessMemory(uint64_t memsize, uint64_t count, uint64_t step) {
	// TODO: Implement (Part P7.1b)

	(void)memsize;
	// Allocate (and deallocate when done) this memory on your program's heap.
	(void)count;
	// In total the function should access (read) the memory count times (read a single uint64 t value from memory each time).
	(void)step;
	// After each read access, the program advances step bytes for the next access, wrapping back to the beginning when it reaches the end of the allocated memory.

	uint64_t mem = malloc(memsize);

	for (int *i; i != count; i += step) {
		if (&i == memsize) {
			// tf we readin???????
			continue;
		} else {
			// To get a value from a memory address in C, you need to use the dereference operator, which is the asterisk (*).
			// First, declare a pointer variable that stores the memory address, then use the dereference operator with the pointer
			// variable to access the value stored at that memory address. For example: `int *ptr = &someVariable; int value = *ptr;`,where `&someVariable` is the memory address of an integer `someVariable`, and `value` will store the data at that address.
			
			// https://stackoverflow.com/questions/15638105/accessing-specific-memory-locations-in-c

			//  Find address.
			char buf[100];
			sprintf(buf, "%" PRIuPTR, (uintptr_t) &x);
			printf("The address of x is %s.\n", buf);

			//  Read the address.
			uintptr_t u;
			sscanf(buf, "%" SCNuPTR, &u);

			//  Convert the integer value to an address.
			int *p = (int *) u;

			print("%p", *i);

		}

	}


	// Example: With memsize=3000, step=1000, and count=10, your program shall read from these offsets in the allocated memory: 0, 1000, 2000, 0, 1000, 2000, 0, 1000, 2000, 0.
	
	// In case of insufficient available memory, your function shall print an error message and return the error code -1. On success, return 0.

	return -1;
}

int accessMemoryWrapper(void *p) {
	// TODO: Implement (Part P7.1c)
	(void)p;
	return -1;
}

// Feel free to adjust: Higher: Better accuracy; Lower: Faster
#define COUNT 10000000
void executeMeasurement() {
	MeasurementParameters params;
	// Print some header...
	printf(" Memory_size ; ");
	printf("step ; cachelines ;     tlbs ; time_duration ; ");
	printf("step ; cachelines ;     tlbs ; time_duration ; ");
	printf("\n");

	// TODO: You may add code here
	while(1) {
		// TODO: You may add code here: Set memory size for this iteration
		params.memsize = 8192; // Change this line!
		printf("%12" PRIu64 " ; ", params.memsize); // Do not change printf's in here!

		// This for loop will execute two times with step=64 and step=4096
		for(uint64_t step = 64; step <= 4096; step *= 64) {
			uint64_t t1 = 0;	// use this variable for your measurement result
			(void)t1;

			// TODO: You may add code here ........... (main part of this function!)

			// Find out number number of cache lines (loc) and number of TLB entires (pages),
			// corresponding to the memory size
			// TODO: Modify the next two lines!
			uint64_t locs = 8192 / step;
			uint64_t pag = 2;

			// Do not change printf's in here!
			printf("%4" PRIu64 " ; %10" PRIu64 " ; %8" PRIu64 " ; %13.8f ; ",
				step, locs, pag, t1*0.000000001);
		}
		printf("\n");
		// TODO: Maybe you add code here as well...

		// And maybe remove this line...
		break;
	}
}




