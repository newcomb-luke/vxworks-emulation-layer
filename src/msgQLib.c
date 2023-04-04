#include "msgQLib.h"
#include "helpers.h"
#include <stddef.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

/********** WARNING **********/
// Unlike every other library implemented in here, this one is written from
// scratch. Linux (and POSIX) do not have unnamed (anonymous) message queues,
// and only support inter-process message queues. That could possibly break a
// few things and also be a hassle to work with. Instead, I opted to not use
// them. This library may not be sound and will require quite a bit of
// validation.

/****************************** Design ***************************************/

//          Sending
//
//   take semSend <----- Facilitates waiting for the queue to have empty space
//                        This semaphore's count is set to maxMsgs when the
//                        queue is created. It counts down as the queue fills up.
//   
//   take mutex
//             
//   do operation
//
//   give mutex
//
//   give semRecv <----- Facilitates other threads waiting for the queue to
//                        have a message in it.
//


//          Receiving
//
//   take semRecv <-----|
//                      |
//   take mutex         |
//                      |
//   do operation       |--- Notice that these are the inverse of Sending
//                      |
//   give mutex         |
//                      |
//   give semSend <-----|
//
// 

// The actual message storage will be allocated on the heap, and accessed
// as a ring buffer to minimize copies.

/************************** Helper structs ***********************************/

// Struct for containing the data required to implement our message queues
typedef struct {
    sem_t semRecv;
    sem_t semSend;
    pthread_mutex_t mutex;

    int numMsgs;
    int maxMsgs;
    int maxMsgLength;
    char* ringBuffer;
    int start;
    int end;
} vxworksMsgQ_t;

/************************ Library functions **********************************/

MSG_Q_ID msgQCreate(int maxMsgs, int maxMsgLength, int options) {
    int status = 0;

    // Step 1: Allocate the struct
    vxworksMsgQ_t* msgQ = (vxworksMsgQ_t*) malloc(sizeof(vxworksMsgQ_t));

    // Step 2: Create semaphores and mutex
    // Receiving sempaphore is given an initial value of 0: no messages to read
    status = sem_init(&(msgQ->semRecv), PTHREAD_PROCESS_PRIVATE, 0);

    if (status != 0) {
        free((void*) msgQ);
        return NULL;
    }
    
    // Sending semaphore is given an initial value of maxMsgs: all can be sent
    status = sem_init(&(msgQ->semSend), PTHREAD_PROCESS_PRIVATE, maxMsgs);

    if (status != 0) {
        sem_destroy(&(msgQ->semRecv));
        free((void*) msgQ);
        return NULL;
    }

    pthread_mutexattr_t attributes;

    status |= pthread_mutexattr_init(&attributes);

    status |= pthread_mutexattr_setpshared(&attributes, PTHREAD_PROCESS_PRIVATE);
    status |= pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_NORMAL);

    status |= pthread_mutex_init(&(msgQ->mutex), &attributes);

    pthread_mutexattr_destroy(&attributes);

    if (status != 0) {
        sem_destroy(&(msgQ->semRecv));
        sem_destroy(&(msgQ->semSend));
        free((void*) msgQ);
        return NULL;
    }

    // Step 3: Allocate the ring buffer
    size_t ringBufferSize = maxMsgs * maxMsgLength;
    msgQ->ringBuffer = (char*) malloc(ringBufferSize);

    // Step 4: Initialize normal data
    msgQ->maxMsgs = maxMsgs;
    msgQ->maxMsgLength = maxMsgLength;
    msgQ->numMsgs = 0;
    msgQ->start = 0;
    msgQ->end = 0;
    
    return (void*) msgQ;
}

STATUS msgQDelete(MSG_Q_ID msgQId) {
    // Remember we have to free TWO blocks of data: the message queue struct,
    // and the ring buffer.

    // The error handling rationale here is that if something fails, the
    // thing still isn't usable, so might as well try to clean up...

    int status = 0;
    vxworksMsgQ_t* msgQ = (vxworksMsgQ_t*) msgQId;

    if (msgQ == NULL) {
        return -1;
    }

    status |= sem_destroy(&(msgQ->semRecv));
    status |= sem_destroy(&(msgQ->semSend));

    status |= pthread_mutex_destroy(&(msgQ->mutex));

    // Free the ring buffer FIRST
    free((void*) msgQ->ringBuffer);

    // THEN the struct
    free((void*) msgQ);

    return status;
}

STATUS msgQSend(MSG_Q_ID msgQId, char* buffer, uint nBytes, int timeout, int priority) {
    // take semSend
    // take mutex
    // do operation
    // give mutex
    // give semRecv

    int status = 0;
    vxworksMsgQ_t* msgQ = (vxworksMsgQ_t*) msgQId;

    if (msgQ == NULL) {
        return -1;
    }
    
    // Wait forever
    if (timeout < 0) {
        if ( 0 != sem_wait(&(msgQ->semSend)) ) {
            return -1;
        }
    }
    // No wait
    else if (timeout == 0) {
        if ( 0 != sem_trywait(&(msgQ->semSend)) ) {
            return -1;
        }
    }
    // Real timeout
    else {
        // This requires an absolute timespec
        struct timespec absTime = _ticksToAbsoluteTimespec(timeout);
        
        if ( 0 != sem_timedwait(&(msgQ->semSend), &absTime) ) {
            return -1;
        }
    }

    // If we have gotten this far, then we have the semaphore
    // We are just going to wait here, if we have the send semaphore, that
    // means that we can send -- so chances are this won't take much time at all

    if ( 0 != pthread_mutex_lock(&(msgQ->mutex)) ) {
        return -1;
    }

    // Now we are free to do our operation

    // Note here that our semaphores ensure that we always have at least one
    // space left

    if (priority == MSG_PRI_NORMAL) {
        char* messageBuffer = msgQ->ringBuffer + (msgQ->end * msgQ->maxMsgLength);
        memcpy(messageBuffer, buffer, nBytes);
        msgQ->end++;
        msgQ->end %= msgQ->maxMsgs;
        msgQ->numMsgs += 1;
    } else {
        msgQ->start--;

        if (msgQ->start < 0) {
            msgQ->start = msgQ->maxMsgs - 1;
        }

        char* messageBuffer = msgQ->ringBuffer + (msgQ->start * msgQ->maxMsgLength);
        memcpy(messageBuffer, buffer, nBytes);
        msgQ->numMsgs += 1;
    }
    
    // If we fail to unlock here, that isn't too much of a problem, everything
    // is still safe
    pthread_mutex_unlock(&(msgQ->mutex));
    // Same with this
    sem_post(&(msgQ->semRecv));

    // Success!
    return 0;
}

STATUS msgQReceive(MSG_Q_ID msgQId, char* buffer, uint maxNBytes, int timeout) {
    // take semRecv
    // take mutex
    // do operation
    // give mutex
    // give semSend

    int status = 0;
    vxworksMsgQ_t* msgQ = (vxworksMsgQ_t*) msgQId;

    if (msgQ == NULL) {
        return -1;
    }

    // Wait forever
    if (timeout < 0) {
        if ( 0 != sem_wait(&(msgQ->semRecv)) ) {
            return -1;
        }
    }
    // No wait
    else if (timeout == 0) {
        if ( 0 != sem_trywait(&(msgQ->semRecv)) ) {
            return -1;
        }
    }
    // Real timeout
    else {
        // This requires an absolute timespec
        struct timespec absTime = _ticksToAbsoluteTimespec(timeout);
        
        if ( 0 != sem_timedwait(&(msgQ->semRecv), &absTime) ) {
            return -1;
        }
    }

    // See msgQSend
    if ( 0 != pthread_mutex_lock(&(msgQ->mutex)) ) {
        return -1;
    }

    // Now we are free to do our operation

    char* messageBuffer = msgQ->ringBuffer + (msgQ->start * msgQ->maxMsgLength);
    memcpy(buffer, messageBuffer, maxNBytes);
    msgQ->start++;
    msgQ->start %= msgQ->maxMsgs;

    // If we fail to unlock here, that isn't too much of a problem, everything
    // is still safe
    pthread_mutex_unlock(&(msgQ->mutex));
    // Same with this
    sem_post(&(msgQ->semSend));

    // Success!
    return 0;
}

int msgQNumMsgs(MSG_Q_ID msgQId) {
    // This is relatively easy, as all we need is the mutex

    vxworksMsgQ_t* msgQ = (vxworksMsgQ_t*) msgQId;
    int numMsgs = -1;

    if (msgQ == NULL) {
        return -1;
    }

    if ( 0 != pthread_mutex_lock(&(msgQ->mutex)) ) {
        return -1;
    }

    numMsgs = msgQ->numMsgs;

    if ( 0 != pthread_mutex_unlock(&(msgQ->mutex)) ) {
        return -1;
    }

    return numMsgs;
}

/************************ Helper functions ***********************************/
