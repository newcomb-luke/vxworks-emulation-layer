#include "msgQLib.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stddef.h>

/************************** Helper structs ***********************************/

typedef struct {
    
} vxworksMsgQ_t;

/************************ Library functions **********************************/

MSG_Q_ID msgQCreate(int maxMsgs, int maxMsgLength, int options) {
    return NULL;
}

STATUS msgQDelete(MSG_Q_ID msgQId) {
    return -1;
}

STATUS msgQSend(MSG_Q_ID msgQId, char* buffer, uint nBytes, int timeout, int priority) {
    return -1;
}

STATUS msgQReceive(MSG_Q_ID msgQId, char* buffer, uint maxNBytes, int timeout) {
    return -1;
}

/************************ Helper functions ***********************************/
