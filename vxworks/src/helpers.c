#include "helpers.h"
#include "sysLib.h"

extern int g_sysClkRate;

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

struct timespec _ticksToAbsoluteTimespec(int ticks) {
    // This requires an *absolute* timestamp, not a relative one. So
    // we need to query the current time
    
    struct timespec currentTime;

    clock_gettime(CLOCK_REALTIME, &currentTime);

    struct timespec relativeTime = _ticksToTimespec(ticks);

    struct timespec absTime;

    absTime.tv_nsec = currentTime.tv_nsec + relativeTime.tv_nsec;
    absTime.tv_sec = currentTime.tv_sec + relativeTime.tv_sec;

    // Keep the timespec valid
    int extraSeconds = absTime.tv_nsec / NANOSECONDS_PER_SECOND;
    absTime.tv_nsec -= extraSeconds * NANOSECONDS_PER_SECOND;
    absTime.tv_sec += extraSeconds;

    return absTime;
}

unsigned long _timespecToTicks(struct timespec spec) {
    double seconds = (double) spec.tv_sec;

    seconds += ((double) spec.tv_nsec) / ((double) NANOSECONDS_PER_SECOND);

    return (unsigned long)( seconds * ((double) g_sysClkRate) );
}

struct timespec _subtractTimespecs(struct timespec before, struct timespec after) {
    struct timespec result;

    time_t seconds = after.tv_sec - before.tv_sec;
    long long nanoseconds = after.tv_nsec - before.tv_nsec;

    if (nanoseconds < 0) {
        seconds--;
        nanoseconds += NANOSECONDS_PER_SECOND;
    }

    result.tv_sec = seconds;
    result.tv_nsec = (size_t) nanoseconds;

    return result;
}
