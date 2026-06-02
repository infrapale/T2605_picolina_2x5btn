// Reads a button attached to a MCP23XXX pin.

// ok to include only the one needed
// both included here to make things simple for example
#include <Adafruit_MCP23X08.h>
#include <Adafruit_MCP23X17.h>

#define BUTTON_PIN 1  // MCP23XXX pin button is attached to
#define BTN_COLS    2
#define BTN_ROWS    5 
#define BTN_NBR     (BTN_COLS * BTN_ROWS)
#define MCP_PIN_NBR     16 
#define BUTT_SHORT_PRESS     4
#define BUTT_LONG_PRESS      8


// only used for SPI
#define CS_PIN 6
#define PIN_I2C0_SDA    (4u)
#define PIN_I2C0_SCL    (5u)
#define PIN_IO_RESET    (12u)
#define PIN_IRQ_A       (14u)
#define PIN_IRQ_B       (15u)

// uncomment appropriate line
//Adafruit_MCP23X08 mcp;
Adafruit_MCP23X17 mcp;

typedef enum
{
    BUTT_STATUS_NOT_AVAIL = 0,
    BUTT_STATUS_IDLE,    
    BUTT_STATUS_SHORT,
    BUTT_STATUS_LONG
} butt_status_et;

typedef struct 
{
    uint8_t         pin;
    char            value;
    butt_status_et  status;        
    uint8_t         debounce;
} butt_st;

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

uint8_t led_pin[2] = {13,14};

void setup() {
    Serial.begin(9600);
    delay(2000);
    while (!Serial); 

    Wire.setSDA(PIN_I2C0_SDA);
    Wire.setSCL(PIN_I2C0_SCL);
    pinMode(PIN_IRQ_A, INPUT);
    pinMode(PIN_IRQ_B, INPUT);
    pinMode(PIN_IO_RESET,OUTPUT);
    digitalWrite(PIN_IO_RESET,LOW); delay(100);
    digitalWrite(PIN_IO_RESET,HIGH); delay(200);
    digitalWrite(PIN_IO_RESET,LOW);

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
    for(uint8_t i = 0; i < 2; i++) mcp.pinMode(led_pin[i], OUTPUT);
    Serial.println("MCP23xxx Button Test!");

  

    // configure pin for input with pull up
    mcp.setupInterrupts(true, true, LOW);
 
    //mcp.setupInterruptPin(7, LOW);
    mcp.digitalWrite(led_pin[0], HIGH);
    Serial.println("Looping...");
}

void loop() {

    uint8_t ioa;
    uint8_t iob;
    uint8_t  butt_x;
    uint16_t butt_x_bm;
    uint16_t btn_bm;
    uint16_t butt_x_pressed;

    //ioa = mcp.readGPIO(0);
    //iob = mcp.readGPIO(1);
    //Serial.printf("%0x - %0x / %d - %d\n", ioa, iob, digitalRead(PIN_IRQ_A), digitalRead(PIN_IRQ_B));
    if (!digitalRead(PIN_IRQ_A)) {
        Serial.print("Interrupt detected on pin: ");
        butt_x = mcp.getLastInterruptPin();
        // btn_bm = mcp.getCapturedInterrupt();
        // butt_x_bm = (1 << butt_x);
        Serial.print(butt_x); Serial.print(" = ");
        Serial.print(butt[butt_x].value);
        butt[butt_x].debounce++;
        if(butt[butt_x].debounce > BUTT_SHORT_PRESS) butt[butt_x].status = BUTT_STATUS_SHORT;
        if(butt[butt_x].debounce > BUTT_LONG_PRESS) butt[butt_x].status = BUTT_STATUS_LONG;
        Serial.printf(" - %d\n", butt[butt_x].status);

        
        //   Serial.print("Pin states at time of interrupt: 0b");
        //   btn_bm = mcp.getCapturedInterrupt();
        //   Serial.println(btn_bm, 2);
        //   Serial.printf("%0x - %0x / %d - %d\n", ioa, iob, digitalRead(PIN_IRQ_A), digitalRead(PIN_IRQ_B));


        delay(250);  // debounce
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
                    butt[i].status = BUTT_STATUS_IDLE;
                    Serial.printf("Short: %c/n",butt[butt_x].value);
                    break;
                case BUTT_STATUS_LONG:
                    butt[i].status = BUTT_STATUS_IDLE;
                    Serial.printf("Long: %c/n",butt[butt_x].value);
                    break;

            }
        }

    }
    delay(100);

  // LOW = pressed, HIGH = not pressed
  // if (!mcp.digitalRead(BUTTON_PIN)) {
  //   Serial.println("Button Pressed!");
  //   delay(250);
  // }
}