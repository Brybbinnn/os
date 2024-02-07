// Authors: abcde fghij [replace with your RU login IDs]
// Group: xzy [replace with your group number]
#include "scheduler.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef int32_t tid_t;

typedef struct _QueueItem {
    /*
     * The data of this item (thread id (thread id)
     */
    tid_t tid;

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
    tid_t threadId;
    ThreadState state;
} Thread;

Thread _threads[MAX_THREADS] = {{0}};

/*
 * The ready queue
 */
Queue queue = {0};

/*
 * Adds a new, waiting thread.
 * The new thread is in state WAITING and not yet inserted in a ready queue.
 */
int startThread(tid_t threadId)
{
    if (((threadId < 0) || (threadId >= MAX_THREADS) ||
        (_threads[threadId].state != STATE_UNUSED))) {
        return -1;
    }

    _threads[threadId].threadId = threadId;
    _threads[threadId].state    = STATE_WAITING;
    return 0;
}

/*
 * Append to the tail of the queue.
 * Does nothing on error.
 */
void _enqueue(Queue *queue, tid_t tid)
{
    (void)queue;
    (void)tid;

    // TODO: Implement
 
    struct _QueueItem *newQueueItem;
    newQueueItem = malloc(sizeof(struct _QueueItem));
    newQueueItem->tid = tid;

    if (queue->head == 0 && queue->tail == NULL) {
        queue->head = newQueueItem; 
        queue->tail = newQueueItem;
    } else {
        struct _QueueItem *current = queue->head;
        while (current->next != 0) {
            current = current->next;
        }
        current->next = newQueueItem;
    }
}

/*
 * Remove and get the head of the queue.
 * Return -1 if the queue is empty.
 */
tid_t _dequeue(Queue *queue) {
    (void)queue;
    QueueItem *tmp;

    // TODO: Implement
    if (queue == 0) {
        return -1;
    }
    else {
        tmp = queue->head;
        queue->head = queue->head->next;
        tid_t value = tmp->tid;
        free(tmp);
        return value;
    }

    return -1;
}

void initScheduler()
{
    // TODO: Implement if you need to initialize any global variables you added
    
}

/*
 * Called whenever a waiting thread gets ready to run.
 */
void onThreadReady(tid_t threadId)
{
    (void)threadId;

    // TODO: Implement

    /*
        Ég veit ekki afhverju &,
        en það virkar? I mean þá
        að það er engin villa...
    */
    if (_threads[threadId].state == STATE_READY) {

    _enqueue(&queue, threadId);
    }
}

/*
 * Called whenever a running thread is forced of the CPU
 * (e.g., through a timer interrupt).
 */
void onThreadPreempted(tid_t threadId)
{
    (void)threadId;
    
    // TODO: Implement

    /*    
        ¯\_(ツ)_/¯
    */

    _threads[threadId].state = STATE_RUNNING;
}

/*
 * Called whenever a running thread needs to wait.
 */
void onThreadWaiting(tid_t threadId)
{
    (void)threadId;

    // TODO: Implement

    /*
        Held ánd MF djóks að þetta sé MÁLIÐ XDDDD
    */
    _threads[threadId].state = STATE_WAITING;
    startThread(threadId);
}

/*
 * Gets the id of the next thread to run and sets its state to running.
 */
// Your scheduler should return -1 if the ready queue is currently empty. Otherwise, 
// it should remove the head of the queue and return the thread ID of the remove element as thread to be executed next
tid_t scheduleNextThread()
{
    // TODO: Implement

    /*
    */
    if (sizeof(queue) == 0) {
        return -1;
    } else {
        tid_t tmp = _dequeue(&queue);
        _threads[tmp].state = STATE_RUNNING;
        return tmp;
    }
}


int main() {
	// Initially empty queue (head and tail is NULL)
	Queue q = {NULL,NULL};

	_enqueue( &q, 42 );
	_enqueue( &q, 99 );
	tid_t x = _dequeue( &q );
	printf("Expect: 42, and I got: %d\n", x);

	x = _dequeue( &q );
	printf("Expect: 99, and I got: %d\n", x);
}
