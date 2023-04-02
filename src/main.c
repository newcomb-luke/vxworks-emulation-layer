#include "vector.h"
#include <stdio.h>

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

    return 0;
}
