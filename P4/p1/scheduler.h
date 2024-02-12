#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <inttypes.h>

/*
 * The maximum priority.
 * Note: 0 is highest priorty, MAX_PRIORITY is the lowest priority
 */
#define MAX_PRIORITY 10 
/*
 * Maximum number of threads
 */
#define MAX_THREADS 20

typedef int32_t tid_t;


void initScheduler();

void onThreadReady(int threadId);

void onThreadPreempted(int threadId);

void onThreadWaiting(int threadId);

int scheduleNextThread();

int startThread(int threadId, int priority);
#endif
