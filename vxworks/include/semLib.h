#pragma once

#include "vxworks.h"

#define SEM_Q_FIFO           0x0
#define SEM_Q_PRIORITY       0x1
#define SEM_DELETE_SAFE      0x4
#define SEM_INVERSION_SAFE   0x8

typedef void* SEM_ID;

typedef enum {
    SEM_EMPTY = 0,
    SEM_FULL = 1
} SEM_B_STATE;

STATUS semGive(SEM_ID sem_id);

STATUS semTake(SEM_ID sem_id, int timeout);

STATUS semFlush(SEM_ID sem_id);

STATUS semDelete(SEM_ID sem_id);

SEM_ID semBCreate(int options, SEM_B_STATE state);

SEM_ID semMCreate(int options);

SEM_ID semCCreate(int options, int count);
