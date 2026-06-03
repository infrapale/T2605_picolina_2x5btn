#ifndef __MAIN_H__
#define __MAIN_H__
#include "WString.h"
#define   __APP__ ((char*)"T2605_picolina_2x5btn")

#define DEBUG_PRINT 
#define SEND_TEST_MSG 
#define MY_ADDR_LEN    8

#include <Arduino.h>

#define SerialX  Serial1

typedef struct
{
    uint32_t next_io_tick;
    char my_addr[MY_ADDR_LEN];
} main_ctrl_st;


typedef struct
{
    char            tag;
    char            addr;         
} modem_data_st;




#endif