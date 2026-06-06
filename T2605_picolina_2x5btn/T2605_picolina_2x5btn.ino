// Reads a button attached to a MCP23XXX pin.

// ok to include only the one needed
// both included here to make things simple for example
// #include <Adafruit_MCP23X08.h>
// #include <Adafruit_MCP23X17.h>
/*
        |   - - - 433 MHz
        |
    -----------
    (9)     (8)
    (7)     (6)
    (5)     (4)
    (3)     (2)
    (1)     (0)
    -----------
        HHH
        HHH
         H      USB
*/




#include    <Wire.h>
#include    "main.h"
#include    "secrets.h"
// #include    <RH_RF69.h>
// #include    "Rfm69Modem.h"
#include    "r69.h"
#include    "atask.h"
#include    "io.h"
#include    "butt.h"


main_ctrl_st main_ctrl = {
    .next_io_tick = 0,
    .my_addr = "BTN1",
};

void modem_task(void);
void print_debug_task(void);

atask_st modem_handle              = {"Radio Modem    ", 100,0, 0, 255, 0, 1, modem_task};
atask_st debug_th                  = {"Debug Task     ", 2000,    0,     0,  255,    0,  1,  print_debug_task };


void setup() {
    io_initialize();
    Serial1.setTX(PIN_TX0);   
    Serial1.setRX(PIN_RX0);
    Serial.begin(115200);
    Serial1.begin(9600);
    delay(2000);
    while (!Serial); 

    Wire.setSDA(PIN_I2C0_SDA);
    Wire.setSCL(PIN_I2C0_SCL);

    atask_initialize();
    butt_initialize();
    r69_initialize(Serial1);



}

butt_status_et status;

void loop() {
    atask_run();

    // char c = butt_read(&status);
    // if(c != '.'){
    //     Serial.printf("%c-%d\n",c, status);
    // }

}