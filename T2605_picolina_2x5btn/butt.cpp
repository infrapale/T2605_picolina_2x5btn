#include <Adafruit_MCP23X08.h>
#include <Adafruit_MCP23X17.h>

#include    "main.h"
#include    "secrets.h"
#include    <RH_RF69.h>
#include    "Rfm69Modem.h"
#include    "atask.h"
#include    "io.h"
#include    "butt.h"

#define MCP_PIN_NBR         16 
#define BUTT_SHORT_PRESS     2
#define BUTT_LONG_PRESS      8

Adafruit_MCP23X17 mcp;


typedef struct 
{
    uint8_t         pin;
    char            value;
    butt_status_et  status;        
    uint8_t         debounce;
} butt_st;

typedef struct
{
    uint8_t pin;
    uint8_t state;

} butt_led_st;

typedef struct 
{
    uint8_t         task_indx;
    butt_status_et  status;
    char            value;
} butt_ctrl_st;


butt_st butt[MCP_PIN_NBR] = 
{
    { 0,  'X', BUTT_STATUS_NOT_AVAIL, 0 },
    { 1,  'X', BUTT_STATUS_NOT_AVAIL, 0 },
    { 2,  'X', BUTT_STATUS_NOT_AVAIL, 0 },
    { 3,  '9', BUTT_STATUS_IDLE, 0 },
    { 4,  '7', BUTT_STATUS_IDLE, 0 },
    { 5,  '5', BUTT_STATUS_IDLE, 0 },
    { 6,  '3', BUTT_STATUS_IDLE, 0 },
    { 7,  '1', BUTT_STATUS_IDLE, 0 },
    { 8,  '0', BUTT_STATUS_IDLE, 0 },
    { 9,  '2', BUTT_STATUS_IDLE, 0 },
    { 10, '4', BUTT_STATUS_IDLE, 0 },
    { 11, '6', BUTT_STATUS_IDLE, 0 },
    { 12, '8', BUTT_STATUS_IDLE, 0 },
    { 13, 'X', BUTT_STATUS_NOT_AVAIL, 0 },
    { 14, 'X', BUTT_STATUS_NOT_AVAIL, 0 },
    { 15, 'X', BUTT_STATUS_NOT_AVAIL, 0 },
};

butt_led_st butt_led[2] = 
{
    { 13, LOW},
    { 14, LOW}
};

void butt_task(void);
atask_st butt_th               = {"RFM69 Task     ", 50,0, 0, 255, 0, 1, butt_task};
//atask_st modem_th            = {"Radio Modem    ", 100,0, 0, 255, 0, 1, modem_task};
butt_ctrl_st butt_ctrl = {0};

void butt_set_led(uint8_t led, uint8_t state)
{
    Serial.printf("set led %d -> %d\n",led,state);
    if (state != LOW) butt_led[led].state = HIGH;
    else butt_led[led].state = LOW;
    mcp.digitalWrite(butt_led[led].pin, butt_led[led].state);
}


void butt_toggle_led(uint8_t led)
{
    if (butt_led[led].state == LOW) butt_led[led].state = HIGH;
    else butt_led[led].state = LOW;
    mcp.digitalWrite(butt_led[led].pin, butt_led[led].state);
}



void butt_initialize(void) 
{   
    if (!mcp.begin_I2C()) {
        Serial.println("Error.");
        while (1);
    }

    for (uint8_t i = 0; i < MCP_PIN_NBR; i++) {  
        if(butt[i].status == BUTT_STATUS_IDLE)
        {
            mcp.pinMode(butt[i].pin, INPUT_PULLUP);
            mcp.setupInterruptPin(butt[i].pin, LOW);
        }
    }
    for(uint8_t i = 0; i < 2; i++) mcp.pinMode(butt_led[i].pin, OUTPUT);
    Serial.println("MCP23xxx Button Test!");
    // configure pin for input with pull up
    mcp.setupInterrupts(true, true, LOW);
    //mcp.setupInterruptPin(7, LOW);

    for (uint8_t i = 0; i < MCP_PIN_NBR; i++) {  
        if(butt[i].status == BUTT_STATUS_IDLE)
        {
            mcp.pinMode(butt[i].pin, INPUT_PULLUP);
            mcp.setupInterruptPin(butt[i].pin, LOW);
        }
    }
    for(uint8_t i = 0; i < 2; i++) mcp.pinMode(butt_led[i].pin, OUTPUT);
    Serial.println("MCP23xxx Button Test!");

    // configure pin for input with pull up
    mcp.setupInterrupts(true, true, LOW);
 
    //mcp.setupInterruptPin(7, LOW);
    //mcp.digitalWrite(butt_led_pin[0], HIGH);
    butt_set_led(BUTT_LED_WHITE, LOW);
    butt_set_led(BUTT_LED_GREEN, LOW);


    butt_ctrl.task_indx =  atask_add_new(&butt_th);

}

char butt_read(butt_status_et *status)
{
    char v = '.';
    if((butt_ctrl.status == BUTT_STATUS_SHORT) || (butt_ctrl.status == BUTT_STATUS_LONG))
    {
        v = butt_ctrl.value;
        *status = butt_ctrl.status;
        butt_ctrl.status = BUTT_STATUS_IDLE;
    }
    return v;
}


void butt_task(void) {

    uint8_t  butt_x;
    uint16_t butt_x_bm;
    uint16_t btn_bm;
    uint16_t butt_x_pressed;

    if (!digitalRead(PIN_IRQ_A)) {
        butt_x = mcp.getLastInterruptPin();
        // btn_bm = mcp.getCapturedInterrupt();
        // butt_x_bm = (1 << butt_x);
        butt[butt_x].debounce++;
        if(butt[butt_x].debounce > BUTT_SHORT_PRESS) butt[butt_x].status = BUTT_STATUS_SHORT;
        if(butt[butt_x].debounce > BUTT_LONG_PRESS) butt[butt_x].status = BUTT_STATUS_LONG;
        // Serial.printf("Interrupt on pin: %d = %c  - %d\n", 
        //     butt_x, 
        //     butt[butt_x].value,
        //     butt[butt_x].status);

        

        // NOTE: If using DEFVAL, INT clears only if interrupt
        // condition does not exist.
        // See Fig 1-7 in datasheet.
        mcp.clearInterrupts();  // clear
    }
    else {
        for (uint8_t i = 0; i < MCP_PIN_NBR; i++) {  
            switch(butt[i].status)
            {
                case BUTT_STATUS_IDLE:
                    break;
                case BUTT_STATUS_SHORT:
                    //Serial.printf("Short: %c\n",butt[i].value);
                    butt_ctrl.status = butt[i].status;
                    butt_ctrl.value  = butt[i].value;
                    butt[i].status = BUTT_STATUS_IDLE;
                    butt[i].debounce = 0;
                    break;
                case BUTT_STATUS_LONG:
                    //Serial.printf("Long: %c\n",butt[i].value);
                    butt_ctrl.status = butt[i].status;
                    butt_ctrl.value  = butt[i].value;
                    butt[i].status = BUTT_STATUS_IDLE;
                    butt[i].debounce = 0;
                    break;

            }

        }

    }

}