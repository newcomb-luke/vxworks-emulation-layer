#include <tickLib.h>
#include "helpers.h"
#include <time.h>
#include <stdlib.h>

// BE WARNED: Global variables

// The number of ticks that happened BEFORE the last update time
unsigned long g_baseKernelTicks = 0;
struct timespec* g_lastUpdateTime = NULL;

unsigned long tickGet(void) {

    if (g_lastUpdateTime == NULL) {
        g_lastUpdateTime = (struct timespec*) malloc(sizeof(struct timespec));

        clock_gettime(CLOCK_BOOTTIME, g_lastUpdateTime);
    }

    struct timespec now;
    clock_gettime(CLOCK_BOOTTIME, &now);

    // In this function, we can rightly (assuming sysLib is implemented
    // correctly) assume that the tick rate has not been modified since
    // the last update time

    struct timespec elapsed = _subtractTimespecs(*g_lastUpdateTime, now);

    return g_baseKernelTicks + _timespecToTicks(elapsed);
}
