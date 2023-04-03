#include "taskLib.h"
#include "sysLib.h"
#include "helpers.h"
#include <stddef.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>

// Helper function definitions
void* _preEntryPoint(void* args);

/************************** Helper structs ***********************************/

// A wrapper struct for the pthread_t just in case we ever need to add more data
typedef struct {
    pthread_t thread;
} vxworksTask_t;

// A struct to hold the real thread entry point and the arguments to it
// See _preEntryPoint for more details
typedef struct {
     FUNCPTR realEntryPt;
     size_t arg1;
     size_t arg2;
     size_t arg3;
     size_t arg4;
     size_t arg5;
     size_t arg6;
     size_t arg7;
     size_t arg8;
     size_t arg9;
     size_t arg10;
} vxworksTaskArgs_t;

/************************ Library functions **********************************/

/**
 * Spawns a new VxWorks style task
 */
TASK_ID taskSpawn
    (
     char* name,
     int priority,
     int options,
     int stackSize,
     FUNCPTR entryPt,
     size_t arg1,
     size_t arg2,
     size_t arg3,
     size_t arg4,
     size_t arg5,
     size_t arg6,
     size_t arg7,
     size_t arg8,
     size_t arg9,
     size_t arg10) {

    // Allocate a new place to store out pthread
    vxworksTask_t* task = (vxworksTask_t*) malloc(sizeof(vxworksTask_t));

    // Maybe the only option we care about is stack size because why not?
    pthread_attr_t attributes;
    pthread_attr_init(&attributes);
    pthread_attr_setstacksize(&attributes, stackSize);

    // This is required to get around the fact that pthreads can only pass
    // "one argument" to the thread. That argument will be a pointer to this
    // structure
    vxworksTaskArgs_t* taskArgs = (vxworksTaskArgs_t*) malloc(sizeof(vxworksTaskArgs_t));
    taskArgs->realEntryPt = entryPt;
    taskArgs->arg1 = arg1;
    taskArgs->arg2 = arg2;
    taskArgs->arg3 = arg3;
    taskArgs->arg4 = arg4;
    taskArgs->arg5 = arg5;
    taskArgs->arg6 = arg6;
    taskArgs->arg7 = arg7;
    taskArgs->arg8 = arg8;
    taskArgs->arg9 = arg9;
    taskArgs->arg10 = arg10;

    // Spawn the thread
    int status = pthread_create(&(task->thread), &attributes, _preEntryPoint, taskArgs);

    // We don't need the thread attributes anymore
    pthread_attr_destroy(&attributes);

    if (status != 0) {
        free((void*) task);
        return NULL;
    }

    return (void*) task;
}

/**
 * Deletes a VxWorks style task
 */
STATUS taskDelete(TASK_ID tid) {
    int status =  pthread_cancel(((vxworksTask_t*)(tid))->thread);

    free(tid);

    return status;
}

/**
 * Delays by the specified VxWorks style ticks
 */
STATUS taskDelay(int ticks) {
    if (ticks < 0) {
        return EINVAL;
    }

    struct timespec spec = _ticksToTimespec(ticks);
    struct timespec remaining;

    int success = EINTR;

    while (success == EINTR) {
        success = nanosleep(&spec, &remaining);

        spec = remaining;
    }

    return success;
}

/************************ Helper functions ***********************************/

/**
 * The entry point of all threads spawned using this API. The purpose of this
 * is to allow multiple (10 to be exact) arguments to be passed to a thread's
 * entry point instead of just one. This reads the void* to the vxworksTaskArgs_t
 * and frees it once it is done. This then calls the real entry point.
 */
void* _preEntryPoint(void* args) {
    // Now we are in a new thread.

    // This is so that we can be cancelled at any time, which matches the behavior
    // of VxWorks.
    int status = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    if (status != 0) {
        return (void*) (uintptr_t) status;
    }

    vxworksTaskArgs_t* taskArgs = (vxworksTaskArgs_t*) args;

    // Yes this looks awful, but this copies all of the values into this
    // function so that we can free the vxworksTaskArgs_t structure
    FUNCPTR entryPt = taskArgs->realEntryPt;
    size_t arg1 = taskArgs->arg1;
    size_t arg2 = taskArgs->arg2;
    size_t arg3 = taskArgs->arg3;
    size_t arg4 = taskArgs->arg4;
    size_t arg5 = taskArgs->arg5;
    size_t arg6 = taskArgs->arg6;
    size_t arg7 = taskArgs->arg7;
    size_t arg8 = taskArgs->arg8;
    size_t arg9 = taskArgs->arg9;
    size_t arg10 = taskArgs->arg10;

    free(args);

    // Call the real thread entry point
    status = entryPt(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);

    return (void*) (uintptr_t) status;
}
