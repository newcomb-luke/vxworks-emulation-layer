#pragma once

#include <time.h>

#define NANOSECONDS_PER_SECOND 1000000000

/**
 * Converts an amount of emulated VxWorks ticks to a timespec
 */
struct timespec _ticksToTimespec(int ticks);

/**
 * Returns a timespec that contains the absolute time of the provided ticks
 * relative to now.
 */
struct timespec _ticksToAbsoluteTimespec(int ticks);
