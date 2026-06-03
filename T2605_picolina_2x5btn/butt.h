#ifndef __BUTT_H__
#define __BUTT_H__

typedef enum
{
    BUTT_STATUS_NOT_AVAIL = 0,
    BUTT_STATUS_IDLE,    
    BUTT_STATUS_SHORT,
    BUTT_STATUS_LONG
} butt_status_et;


void butt_initialize(void);

char butt_read(butt_status_et *status);

#endif