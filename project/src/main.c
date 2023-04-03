#include "vector.h"
#include <stdio.h>
#include <taskLib.h>
#include <semLib.h>
#include <stdlib.h>
#include <msgQLib.h>

int runMe(size_t argA, size_t argSemBin, size_t argMsgQ);

int main() {

    vector vec;

    vector_init(&vec, sizeof(int));

    for (int i = 0; i < 4; i++) {
        vector_push(&vec, &i);
    }

    int n = 7;
    vector_push(&vec, &n);

    for (int i = 0; i < 5; i++) {
        int* vPtr = (int*)vector_get(&vec, i);

        if (vPtr == NULL) {
            printf("Invalid index requested\n");
            break;
        }

        printf("%d\n", *vPtr);
    }

    vector_delete(&vec);

    SEM_ID semBin = semBCreate(0, SEM_EMPTY);

    if (semBin == NULL) {
        printf("Failed to create semaphore\n");
        exit(-1);
    }

    MSG_Q_ID msgQ = msgQCreate(10, sizeof(int), MSG_Q_FIFO);

    if (msgQ == NULL) {
        printf("Failed to create message queue\n");
        exit(-1);
    }

    TASK_ID task = taskSpawn("", 100, 0, 0x2000, (FUNCPTR) runMe,
                                            5,
                                            (size_t) semBin,
                                            (size_t) msgQ,
                                            0,
                                            0,
                                            0,
                                            0,
                                            0,
                                            0,
                                            0);

    printf("Hello from main thread\n");

    taskDelay(500);

    printf("Hello again from main thread\n");

    taskDelay(500);

    semGive(semBin);

    taskDelay(500);

    for (int i = 0; i < 9; i++) {
        int b = i * 3;
        msgQSend(msgQ, (char*) &b, sizeof(int), WAIT_FOREVER, MSG_PRI_NORMAL);
    }

    int c = 69;
    msgQSend(msgQ, (char*) &c, sizeof(int), WAIT_FOREVER, MSG_PRI_URGENT);

    c = 10;
    msgQSend(msgQ, (char*) &c, sizeof(int), WAIT_FOREVER, MSG_PRI_NORMAL);

    taskDelay(500);

    semGive(semBin);

    printf("Parent task is done!\n");

    // Would not be required on VxWorks, since tasks detach
    for (;;) {}

    return 0;
}

int runMe(size_t argA, size_t argSemBin, size_t argMsgQ) {
    int a = (int) argA;
    SEM_ID semBin = (SEM_ID) argSemBin;
    MSG_Q_ID msgQ = (MSG_Q_ID) argMsgQ;

    printf("Waiting to print...\n");

    semTake(semBin, WAIT_FOREVER);

    printf("Number that I was given: %d\n", a);

    // semTake(semBin, WAIT_FOREVER);

    for (int i = 0; i < 11; i++) {
        int b = 0;

        msgQReceive(msgQ, (char*) &b, sizeof(int), WAIT_FOREVER);

        printf("Received: %d\n", b);
    }

    printf("task runMe is done!\n");

    msgQDelete(msgQ);

    return 0;
}
