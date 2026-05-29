// Reads a button attached to a MCP23XXX pin.

// ok to include only the one needed
// both included here to make things simple for example
#include <Adafruit_MCP23X08.h>
#include <Adafruit_MCP23X17.h>

#define BUTTON_PIN 1  // MCP23XXX pin button is attached to
#define BTN_COLS    2
#define BTN_ROWS    5 

// only used for SPI
#define CS_PIN 6

// uncomment appropriate line
//Adafruit_MCP23X08 mcp;
Adafruit_MCP23X17 mcp;

uint8_t btn_pin[BTN_COLS][BTN_ROWS] =
{
    {0,1,2,3,4},
    {8,9,10,11,12}
};

uint8_t led_pin[2] = {13,14};

void setup() {
  for (uint8_t col = 0; col < BTN_COLS; col++) {
      for(uint8_t row = 0; row < BTN_ROWS; row++){
          mcp.pinMode(btn_pin[col][row], INPUT_PULLUP);

      }
  }
  for(uint8_t i = 0; i < 2; i++) mcp.pinMode(led_pin[i], OUTPUT);
  Serial.begin(9600);
  //while (!Serial);
  Serial.println("MCP23xxx Button Test!");

  // uncomment appropriate mcp.begin
  if (!mcp.begin_I2C()) {
  //if (!mcp.begin_SPI(CS_PIN)) {
    Serial.println("Error.");
    while (1);
  }

  // configure pin for input with pull up
  mcp.pinMode(BUTTON_PIN, INPUT_PULLUP);
  mcp.digitalWrite(led_pin[0], HIGH);
  Serial.println("Looping...");
}

void loop() {

  uint8_t ioa = mcp.readGPIO(0);
  uint8_t iob = mcp.readGPIO(1);
  Serial.printf("%d - %d\n");
  delay(100);
  // LOW = pressed, HIGH = not pressed
  // if (!mcp.digitalRead(BUTTON_PIN)) {
  //   Serial.println("Button Pressed!");
  //   delay(250);
  // }
}