#pragma once

#include "vxworks.h"
#include <sys/types.h>

typedef void* MSG_Q_ID;

#define MSG_Q_FIFO      0x00
#define MSG_Q_PRIORITY  0x01

#define MSG_PRI_NORMAL  0
#define MSG_PRI_URGENT  1

/**
 * Creates and initializes a VxWorks style message queue
 */
MSG_Q_ID msgQCreate(int maxMsgs, int maxMsgLength, int options);

/**
 * Deletes a VxWorks style message queue
 */
STATUS msgQDelete(MSG_Q_ID msgQId);

/**
 * Sends a message on a message queue
 */
STATUS msgQSend(MSG_Q_ID msgQId, char* buffer, uint nBytes, int timeout, int priority);

/**
 * Receives a message on a message queue
 */
STATUS msgQReceive(MSG_Q_ID msgQId, char* buffer, uint maxNBytes, int timeout);

/**
 * Get the number of messages queued to a message queue, or -1 if error
 */
int msgQNumMsgs(MSG_Q_ID msgQId);
