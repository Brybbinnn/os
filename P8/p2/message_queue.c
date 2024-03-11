#include "message_queue.h"
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <stdlib.h>
#include <stdio.h>

/*
 * The commands supported by the server
 */
typedef enum _Command {
    CmdAdd = 0x00,     // Adds the two message parameters
    CmdMul,            // Multiples the two message parameters
    CmdExit            // Stops the server
} Command;

/*
 * The message format to be sent to the server.
 */
typedef struct _Message {
    /*
     * One of the command constants.
     */
    Command command;
    /*
     * Used as operand 1 (if required)
     */
    int parameter1;
    /*
     * Used as operand 2 (if required)
     */
    int parameter2;
} Message;

#define FORMAT_STRING_ADD      "Calc: %d + %d = %d\n"
#define FORMAT_STRING_MUL      "Calc: %d * %d = %d\n"

// Initialize sending messages to the server
mqd_t startClient(void)
{
    mqd_t client = mq_open(QUEUE_NAME, O_WRONLY);
    if (client == (mqd_t)-1) {
        return -1;
    }
    return client;
}

// Send a command to server to terminate it
int sendExitTask(mqd_t client)
{
    Message msg = {CmdExit, 0, 0};
    if (mq_send(client, (const char*)&msg, sizeof(msg), 0) == -1) {
        return -1;
    }

    stopClient(client);

    return 0;
}

// Send a command to server to add two numbers
int sendAddTask(mqd_t client, int operand1, int operand2)
{
    Message msg = {CmdAdd, operand1, operand2};
    if (mq_send(client, (const char*)&msg, sizeof(msg), 0) == -1) {
        return -1;
    }
    return 0;
}


// Send a command to server to multiply two numbers
int sendMulTask(mqd_t client, int operand1, int operand2)
{
    Message msg = {CmdMul, operand1, operand2};
    if (mq_send(client, (const char*)&msg, sizeof(msg), 0) == -1) {
        perror("mq_send (mul)");
        return -1;
    }
    return 0;
}

// Close the message queue used by the client
int stopClient(mqd_t client)
{
    if (mq_close(client) == -1) {
        return -1;
    }
    return 0;
}

// Server proccesses incoming messages
int runServer(void)
{
    int didExit = 0, hadError = 0;
    Message msg;

    struct mq_attr attr;
    attr.mq_flags   = 0;
    attr.mq_maxmsg  = 10;           // Maximum number of messages in the queue
    attr.mq_msgsize = sizeof(msg);  // Maximum message size
    attr.mq_curmsgs = 0;
    (void) attr;

    mqd_t server = mq_open(QUEUE_NAME, O_CREAT | O_EXCL | O_RDONLY, 0644, &attr);
    if(server == (mqd_t)-1) {
        return -1;
    }

    do {
        // Attempt to receive a message from the queue.
        ssize_t received = mq_receive(server, (char*)&msg, sizeof(msg), NULL);
        if (received != sizeof(msg)) {
            // This implicitly also checks for error (i.e., -1)
            hadError = 1;
            continue;
        }

        switch (msg.command)
        {
            case CmdExit:
                // End this loop.
                didExit = 1;
                break;

            case CmdAdd:
                // Print the required output.
                printf(FORMAT_STRING_ADD,
                       msg.parameter1,
                       msg.parameter2,
                       msg.parameter1 + msg.parameter2);
                break;

            case CmdMul:
                // Print the required output.
                printf(FORMAT_STRING_MUL,
                       msg.parameter1,
                       msg.parameter2,
                       msg.parameter1 * msg.parameter2);
                break;

            default:
                break;
        }
    } while (!didExit);

    // Clean uð: close the message queue and unlink it.
    if (mq_close(server) == -1) {
        hadError = 1;
    }

    if (mq_unlink(QUEUE_NAME) == -1) {
        hadError = 1;
    }

    return hadError ? -1 : 0;
}
