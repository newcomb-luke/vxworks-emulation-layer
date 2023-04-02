#include "sysLib.h"

#define DEFAULT_SYS_CLK_RATE 500

int g_sysClkRate = DEFAULT_SYS_CLK_RATE;

int sysClkRateGet(void) {
    return g_sysClkRate;
}


STATUS sysClkRateSet(int ticksPerSecond) {
    g_sysClkRate = ticksPerSecond;
}
