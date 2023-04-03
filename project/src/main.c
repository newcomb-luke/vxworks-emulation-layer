#include "vector.h"
#include <stdio.h>
#include <taskLib.h>
#include <semLib.h>
#include <stdlib.h>

int runMe(size_t argA, size_t argSemBin);

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

    TASK_ID task = taskSpawn("", 100, 0, 0x2000, (FUNCPTR) runMe,
                                            5,
                                            (size_t) semBin,
                                            0,
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

    // Would not be required on VxWorks, since tasks detach
    for (;;) {}

    return 0;
}

int runMe(size_t argA, size_t argSemBin) {
    int a = (int) argA;
    SEM_ID semBin = (SEM_ID) argSemBin;

    printf("Waiting to print...\n");

    semTake(semBin, WAIT_FOREVER);

    printf("Number that I was given: %d\n", a);
    
    semDelete(semBin);

    return 0;
}
