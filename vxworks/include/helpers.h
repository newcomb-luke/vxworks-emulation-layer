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

/**
 * Converts a timespec to an amount of emulated VxWorks ticks based on the
 * emulated sysClkRate
 */
unsigned long _timespecToTicks(struct timespec spec);

/**
 * Helper function to subtract two timespecs
 */
struct timespec _subtractTimespecs(struct timespec before, struct timespec after);
