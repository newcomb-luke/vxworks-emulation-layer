#pragma once

#include <time.h>

#define NANOSECONDS_PER_SECOND 1000000000

struct timespec _ticksToTimespec(int ticks);
