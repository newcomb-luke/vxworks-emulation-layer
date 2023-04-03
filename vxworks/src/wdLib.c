#include "wdLib.h"
#include <signal.h>
#include <time.h>

/************************** Helper structs ***********************************/

typedef struct {
    timer_t timer;
} vxworksWdTimer_t;

/************************ Library functions **********************************/

WDOG_ID wdCreate(void) {
    return NULL;
}

STATUS wdDelete(WDOG_ID wdId) {
    return -1;
}

STATUS wdStart(WDOG_ID wdId, int delay, FUNCPTR pRoutine, int parameter) {
    return -1;
}

STATUS wdCancel(WDOG_ID wdId) {
    return -1;
}
