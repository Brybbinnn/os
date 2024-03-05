#define _POSIX_C_SOURCE 199309L

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "measure_tlb.h"

#define PAGESIZE 4096

int64_t measureFunction( int(*function)(void *), void *arg ) { // A

	(void)function;
	(void)arg;

// 	Invokes the function referenced by function, passing arg as parameter;
// before and after the invocation, uses the API function clock gettime (see man
// page) to obtain a high-precision time stamp; and finally
// if function returns an error code (value < 0), returns -1;
// otherwise, returns the elapsed time (difference between the two time stamps) in
// nanoseconds.

	struct timespec start, end;

    // Get start time
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Invoke the function

    if (function(arg) < 0) {
		return -1;
	}

    // Get end time
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate elapsed time in nanoseconds
	double elapsed_ns;
	elapsed_ns = (end.tv_sec - start.tv_sec);
    elapsed_ns = (elapsed_ns + (end.tv_nsec - start.tv_nsec));

    return elapsed_ns;
}

int accessMemory(uint64_t memsize, uint64_t count, uint64_t step) { // B

	void *allocated_memory = malloc(memsize);

	// If memory allocation kaput
	if (allocated_memory == NULL) {
		return -1;
	}

	// Cast to char to access memory
	char *memory = (char *)allocated_memory;
	uint64_t index = 0;
	char value = 0;

	for (uint64_t i = 0; i < count; i++) {
		value = memory[index];
		index = (index + step) & (memsize - 1);
	}
	free(memory);
		(void)value;
	return 0;
}

int accessMemoryWrapper(void *p) { // C

	(void)p;
	// Cast the void* parameter to MeasurementParameters*
    MeasurementParameters *param = (MeasurementParameters*)p;

    // Extract parameters from the structure
    uint64_t memsize = param->memsize;
    uint64_t count = param->count;
    uint64_t step = param->step;

    // Call the original function with extracted parameters
    int access = accessMemory(memsize, count, step);

    // Return the result of the accessMemory function
    return access;
}

// Feel free to adjust: Higher: Better accuracy; Lower: Faster
#define COUNT 10000000
#define MEMORY_LIMIT (1ULL << 30)  // 1 GiB

void executeMeasurement() { // D
	MeasurementParameters params;
	// Print some header...
	printf(" Memory_size ; ");
	printf("step ; cachelines ;     tlbs ; time_duration ; ");
	printf("step ; cachelines ;     tlbs ; time_duration ; ");
	printf("\n");

	params.memsize = PAGESIZE * 2; // I changed this line!
	params.count = COUNT;
	// I added code here!
	while(params.memsize <= MEMORY_LIMIT) {
		// TODO: You may add code here: Set memory size for this iteration
		printf("%12" PRIu64 " ; ", params.memsize); // I did not change the printf's in here!

		// This for loop will execute two times with step=64 and step=4096
		for(uint64_t step = 64; step <= 4096; step *= 64) {
			// use this variable for your measurement result

			// TODO: You may add code here ........... (main part of this function!)

			params.step = step;

			int (*fptr)(void *) = accessMemoryWrapper;

			int64_t t1 = measureFunction(fptr, &params);

			// Find out number number of cache lines (locs) and number of TLB entries (pages),
			// corresponding to the memory size
			uint64_t locs = params.memsize / step;
			uint64_t pag = params.memsize / PAGESIZE;

			// Do not change printf's in here!
			printf("%4" PRIu64 " ; %10" PRIu64 " ; %8" PRIu64 " ; %13.8f ; ",
				step, locs, pag, t1*0.000000001);
		}
		printf("\n");
		// TODO: Maybe you add code here as well...
		// Maybe I increased the memory size for the next iteration...
		params.memsize *= 2;

		// And maybe remove this line... or maybe not...
	}
}