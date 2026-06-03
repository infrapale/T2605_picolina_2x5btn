
#include    "main.h"
#include    "secrets.h"
#include    <RH_RF69.h>
#include    "Rfm69Modem.h"
#include    "atask.h"
#include    "io.h"
#include    "r69.h"
#include    "butt.h"


#define MY_MODULE_TAG   'B'
#define MY_MODULE_ADDR  '1'
#define ENCRYPTKEY    RFM69_KEY   // defined in secret.h

typedef struct
{
    uint8_t task_indx;
    char butt_val;
    butt_status_et butt_status;
    char    buff[64];
    uint16_t duration;

} r69_st;

uint8_t key[] = RFM69_KEY;

RH_RF69         rf69(PIN_RFM_CS, PIN_RFM_IRQ);
Rfm69Modem      rfm69_modem(&rf69,  PIN_RFM_RESET, -1 );
//modem_data_st   modem_data = {MY_MODULE_TAG, MY_MODULE_ADDR};

extern main_ctrl_st main_ctrl;
r69_st r69;

void modem_task(void)
{
    rfm69_modem.modem_task();
}


void r69_task(void);
atask_st r69_th                = {"RFM69 Task     ", 100,0, 0, 255, 0, 1, r69_task};
atask_st modem_th              = {"Radio Modem    ", 100,0, 0, 255, 0, 1, modem_task};
//atask_st modem_th            = {"Radio Modem    ", 100,0, 0, 255, 0, 1, modem_task};

void r69_initialize(Stream &s)
{
    rfm69_modem.initialize(MY_MODULE_TAG, MY_MODULE_ADDR, key);
    atask_add_new(&modem_th);
    rfm69_modem.radiate(__APP__);

    rfm69_modem.set_serial(s);
    r69.task_indx =  atask_add_new(&r69_th);
}

void r69_task(void)
{
    switch(r69_th.state)
    {
        case 0:
            r69_th.state = 10;
            break;

        case 10:
            r69.butt_val = butt_read(&r69.butt_status);
            if(r69.butt_val != '.'){
                Serial.printf("%c-%d\n", r69.butt_val, r69.butt_status);
                r69_th.state = 20;
            }
            break;   
        case 20:
            switch(r69.butt_val)
            {
                case '0':
                    r69.duration = 0;
                    break;
                case '1':
                    r69.duration = 1;
                    break;
                case '2':    
                    r69.duration = 1800;
                    break;
                case '3':    
                    r69.duration = 3600;
                    break;
                case '4':    
                    r69.duration = 7200;
                    break;
            }
            sprintf(r69.buff,"<R;RANTA;%s;PUMP;%d>", main_ctrl.my_addr, r69.duration);
            Serial.println(r69.buff);    
            //Serial1.println(r69.buff);    
            rfm69_modem.radiate(r69.buff);
            r69_th.state = 10;
            break;
    }
}