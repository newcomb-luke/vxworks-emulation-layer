#pragma once

#include "vxworks.h"

typedef void* WDOG_ID;

/**
 * Create a new VxWorks style watchdog timer
 */
WDOG_ID wdCreate(void);

/**
 * Delete a watchdog timer
 */
STATUS wdDelete(WDOG_ID wdId);

/**
 * Start a watchdog timer
 */
STATUS wdStart(WDOG_ID wdId, int delay, FUNCPTR pRoutine, int parameter);

/**
 * Cancel a currently counting watchdog
 */
STATUS wdCancel(WDOG_ID wdId);
