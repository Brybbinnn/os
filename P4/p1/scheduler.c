// Authors: bryndisg21@ru.is viggoh21@ru.is
// Group: 66
#include "scheduler.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


typedef struct _QueueItem {
    /*
     * The tid of this item.
     */
    int tid;
    /*
     * The next item in the queue.
     * NULL if there is no next item.
     */
    struct _QueueItem *next;
} QueueItem;

typedef struct _Queue {
    /*
     * The first item of the queue.
     * NULL if the queue is empty.
     */
    QueueItem *head;
    /*
     * The last item of the queue.
     * undefined if the queue is empty.
     */
    QueueItem *tail;
} Queue;

typedef enum _ThreadState {
    STATE_UNUSED = 0, // This entry in the _threads array is unused.

    STATE_READY,      // The thread is ready and should be on a ready queue for
                      //   selection by the scheduler
    STATE_RUNNING,    // The thread is running and should not be on a ready queue
    STATE_WAITING     // The thread is blocked and should not be on a ready queue
} ThreadState;

typedef struct _Thread {
    int threadId;
    ThreadState state;
    /*
     * Range: 0 ... MAX_PRIORITY (including)
     * 0 is highest, MAX_PRIORITY is lowest priority
     */
    int priority;
} Thread;

Thread _threads[MAX_THREADS] = {{0}};

/* TODO: Add gloabl variables if needed*/
int consec[MAX_PRIORITY]; // Consecutive counter for starvation
Queue priorityQueues[MAX_PRIORITY] = {{NULL, NULL}}; // Array of queues for each priority level

/*
 * Adds a new, waiting thread.
 */
int startThread(int threadId, int priority) {
    if (((threadId < 0) || (threadId >= MAX_THREADS) ||
        (_threads[threadId].state != STATE_UNUSED)) ||
        (priority < 0) || (priority >= MAX_PRIORITY)) {
        return -1;
    }
    _threads[threadId].threadId = threadId;
    _threads[threadId].state = STATE_WAITING;
    _threads[threadId].priority = priority;
    return 0;
}

/*
 * Append to the tail of the queue.
 * Does nothing on error.
 */
void _enqueue(Queue *queue, int tid) {

    assert(queue != NULL);
    QueueItem *item = (QueueItem*)malloc(sizeof(QueueItem));

    if (item == NULL) {
        return;
    }

    item->tid = tid;
    item->next = NULL;

    if (queue->head == NULL) {
        queue->head = item;
    } else {
        assert(queue->tail != NULL);
        queue->tail->next = item;
    }
    
    queue->tail = item;
}

int _dequeue(Queue *queue) {

    assert(queue != NULL);

    if (queue->head == NULL) {
        return -1;
    }

    QueueItem *head = queue->head;
    int tid = head->tid;
    queue->head = head->next;
    free(head);
    return tid;
}

void initScheduler() {
    // TODO: Implement if you need to initialize any global variables you add
    for (int i = 0; i < MAX_PRIORITY; ++i) {
        Queue queue = {0};
        priorityQueues[i] = queue;
    }
}

static void _enqueueThread(int threadId) {
    assert((threadId >= 0) && (threadId < MAX_THREADS));
    assert(_threads[threadId].state == STATE_READY);
    int priority = _threads[threadId].priority;
    assert(priority >= 0 && priority < MAX_PRIORITY);
    
    // Append a thread to the right ready queue.
    _enqueue(&priorityQueues[priority], threadId);
}

/*
 * Called whenever a waiting thread gets ready to run.
 */
void onThreadReady(int threadId) {
    assert((threadId >= 0) && (threadId < MAX_THREADS));
    assert(_threads[threadId].state == STATE_WAITING);

    _threads[threadId].state = STATE_READY;
    _enqueueThread(threadId);
}

/*
 * Called whenever a running thread is forced of the CPU
 * (e.g., through a timer interrupt).
 */
void onThreadPreempted(int threadId) {
    assert((threadId >= 0) && (threadId < MAX_THREADS));
    _threads[threadId].state = STATE_READY;
    _enqueueThread(threadId);
}

/*
 * Called whenever a running thread needs to wait.
 */
void onThreadWaiting(int threadId) {
    assert((threadId >= 0) && (threadId < MAX_THREADS));
    _threads[threadId].state = STATE_WAITING;
}
/*
 * Gets the id of the next thread to run and sets its state to running.
 */
int scheduleNextThread()
{
    int tid = 0;

    // checks if the scheduler is starving low priority processes
    for(int i = 0; i<MAX_PRIORITY; ++i){
        if(priorityQueues[i].head != NULL){
            if(consec[i] == 4){
                consec[i] = 0;
                continue; 
            }

            tid = _dequeue(&priorityQueues[i]);
            consec[tid]++;
            break;
        }
        else if(priorityQueues[i].head == NULL){
            consec[i] = 0;
        }
        // If we've checked all priority queues without finding a suitable thread
        // we reset the consec counter for each priority and return -1
        if(i == MAX_PRIORITY-1){
            for(int i = 0; i<MAX_PRIORITY; i++){
                consec[i] = 0;
            }
            return -1;
        }
    }

    _threads[tid].state = STATE_RUNNING;
    return tid;
}

#if 0
// int main() {
//     // Initially empty queue (head and tail is NULL)
//     Queue q = {NULL,NULL};
//     _enqueue( &q, 42 );
//     _enqueue( &q, 99 );
//     int x = _dequeue( &q );
//     printf("Expect: 42, and I got: %d\n", x);
//     x = _dequeue( &q );
//     printf("Expect: 99, and I got: %d\n", x);
// }
#endif