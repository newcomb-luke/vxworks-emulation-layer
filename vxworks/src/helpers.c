#include "helpers.h"
#include "sysLib.h"

/**
 * Converts a tick value to a timespec structure
 */
struct timespec _ticksToTimespec(int ticks) {

    double ticksFloat = (double) ticks;
    double clkRate = (double) sysClkRateGet();
    double seconds = ticksFloat / clkRate;

    time_t sec = (time_t) seconds;

    double nanoseconds = (seconds - (double) sec) * NANOSECONDS_PER_SECOND;

    long nsec = (long) nanoseconds;

    struct timespec spec;

    spec.tv_sec = sec;
    spec.tv_nsec = nsec;

    return spec;
}
