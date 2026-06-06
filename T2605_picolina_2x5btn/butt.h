#ifndef __BUTT_H__
#define __BUTT_H__

typedef enum
{
    BUTT_STATUS_NOT_AVAIL = 0,
    BUTT_STATUS_IDLE,    
    BUTT_STATUS_SHORT,
    BUTT_STATUS_LONG
} butt_status_et;

typedef enum
{
    BUTT_LED_WHITE = 0,
    BUTT_LED_GREEN,
    BUTT_LED_NBR_OF
} butt_led_et;

void butt_initialize(void);

char butt_read(butt_status_et *status);

void butt_set_led(uint8_t led, uint8_t state);

void butt_toggle_led(uint8_t led);


#endif