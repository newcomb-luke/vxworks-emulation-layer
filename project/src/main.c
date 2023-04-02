#include "vector.h"
#include <stdio.h>
#include "taskLib.h"

int runMe(int a);

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

    TASK_ID task = taskSpawn("", 100, 0, 0x2000, (FUNCPTR) runMe,
                                            5,
                                            0,
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

    taskDelete(task);

    // Would not be required on VxWorks, since tasks detach
    for (;;) {}

    return 0;
}

int runMe(int a) {
    printf("Number that I was given: %d\n", a);

    taskDelay(5000);

    printf("I lived!\n");

    return 0;
}