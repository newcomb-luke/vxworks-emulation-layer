#pragma once

#include "vxworks.h"

int sysClkRateGet(void);
STATUS sysClkRateSet(int ticksPerSecond);
