#include "semLib.h"
#include "helpers.h"
#include <pthread.h>
#include <stddef.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**************************** Helper types ***********************************/

typedef enum {
    SEM_BIN = 0,
    SEM_MUTEX = 1,
    SEM_COUNT = 2
} SEM_TYPE;

typedef union {
    sem_t sem;
    pthread_mutex_t mutex;
} vxworksSemInner_t;

// Wrapper struct over a posix semaphore/mutex
typedef struct {
    SEM_TYPE type;
    vxworksSemInner_t inner;

} vxworksSem_t;

/************************ Library functions **********************************/

STATUS semGive(SEM_ID sem_id) {
    vxworksSem_t* sem = (vxworksSem_t*) sem_id;
    int status = 0;

    // If we are a binary semaphore, the count should never go over 1
    if (sem->type == SEM_BIN) {
        int semaphoreValue = 0;

        status = sem_getvalue(&(sem->inner.sem), &semaphoreValue);

        if (status != 0) {
            return status;
        }

        // If any only if the semaphore's value is 0 do we give it
        if (semaphoreValue <= 0) {
            status = sem_post(&(sem->inner.sem));
        }

    }
    // If we are a counting semaphore, we don't really need to care
    else if (sem->type == SEM_COUNT) {
        status = sem_post(&(sem->inner.sem));
    }
    // If we are a mutex, we need to do something completely different
    else {
        status = pthread_mutex_unlock(&(sem->inner.mutex));
    }

    return status;
}

STATUS semTake(SEM_ID sem_id, int timeout) {
    vxworksSem_t* sem = (vxworksSem_t*) sem_id;
    int status = 0;

    // If we are a binary or counting semaphore
    if (sem->type == SEM_BIN || sem->type == SEM_COUNT) {
        // Here we do different things depending on the value of timeout
        
        // Wait forever
        if (timeout < 0) {
            status = sem_wait(&(sem->inner.sem));
        }
        // Don't wait at all
        else if (timeout == 0) {
            status = sem_trywait(&(sem->inner.sem));
        }
        // Wait for a specific timeout
        else {
            // This requires an absolute timespec
            struct timespec absTime = _ticksToAbsoluteTimespec(timeout);
            
            status = sem_timedwait(&(sem->inner.sem), &absTime);
        }
    }
    // If we are a mutex
    else {
        // Wait forever
        if (timeout < 0) {
            status = pthread_mutex_lock(&(sem->inner.mutex));
        }
        // Don't wait at all
        else if (timeout == 0) {
            status = pthread_mutex_trylock(&(sem->inner.mutex));
        }
        // Wait for a specific timeout
        else {
            // This requires an absolute timespec
            struct timespec absTime = _ticksToAbsoluteTimespec(timeout);

            status = pthread_mutex_timedlock(&(sem->inner.mutex), &absTime);
        }
    }

    return status;
}

STATUS semDelete(SEM_ID sem_id) {
    vxworksSem_t* sem = (vxworksSem_t*) sem_id;
    int status = 0;

    // If we are a binary or counting semaphore
    if (sem->type == SEM_BIN || sem->type == SEM_COUNT) {
        status = sem_destroy(&(sem->inner.sem));
    }
    // If we are a mutex
    else {
        status = pthread_mutex_destroy(&(sem->inner.mutex));
    }
    
    free((void*) sem);

    return status;
}

SEM_ID semBCreate(int options, SEM_B_STATE state) {
    // FIXME Not sure what the VxWorks behavior is on this
    int semState = SEM_EMPTY;

    if (state > 0) {
        semState = SEM_FULL;
    } else if (state < 0) {
        return NULL;
    }

    vxworksSem_t* sem = (vxworksSem_t*) malloc(sizeof(vxworksSem_t));
    sem->type = SEM_BIN;

    int status = sem_init(&(sem->inner.sem), PTHREAD_PROCESS_PRIVATE, semState);

    if (status != 0) {
        free((void*) sem);
        return NULL;
    }

    return (void*) sem;
}

SEM_ID semMCreate(int options) {
    vxworksSem_t* sem = (vxworksSem_t*) malloc(sizeof(vxworksSem_t));
    sem->type = SEM_MUTEX;

    int status = 0;

    pthread_mutexattr_t attributes;

    status |= pthread_mutexattr_init(&attributes);

    status |= pthread_mutexattr_setpshared(&attributes, PTHREAD_PROCESS_PRIVATE);
    status |= pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_NORMAL);

    status |= pthread_mutex_init(&(sem->inner.mutex), &attributes);

    pthread_mutexattr_destroy(&attributes);

    if (status != 0) {
        free((void*) sem);
        return NULL;
    }

    return (void*) sem;
}

SEM_ID semCCreate(int options, int count) {
    if (count < 0) {
        return NULL;
    }

    vxworksSem_t* sem = (vxworksSem_t*) malloc(sizeof(vxworksSem_t));
    sem->type = SEM_COUNT;

    int status = sem_init(&(sem->inner.sem), PTHREAD_PROCESS_PRIVATE, count);

    if (status != 0) {
        free((void*) sem);
        return NULL;
    }

    return (void*) sem;
}
