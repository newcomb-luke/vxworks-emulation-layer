#pragma once

#include "vxworks.h"
#include <stdint.h>
#include <stddef.h>

typedef void* TASK_ID;

#define VX_FP_TASK        0x0008
#define VX_PRIVATE_ENV    0x0080
#define VX_NO_STACK_FILL  0x0100
#define VX_UNBREAKABLE    0x0002

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
     size_t arg10
    );

/**
 * Deletes a VxWorks style task
 */
STATUS taskDelete(TASK_ID tid);

/**
 * Delays by the specified VxWorks style ticks
 */
STATUS taskDelay(int ticks);
