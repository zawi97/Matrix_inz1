#include "Arduino.h"
#include "esp.h"

#include <ESP8266WiFi.h>
#include <ModbusIP_ESP8266.h>
#include <FastLED.h>

//#include <WiFi_login.h>

#define LED_PIN  5
#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define BRIGHTNESS 32

// Modbus Registers Offsets
const int MODE_HREG = 1;
const int LENGTH_HREG = 2;
const int PIXEL_HREG = 10;
// Params for width and height
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 1;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
struct CRGB leds[NUM_LEDS];
//CRGB* const leds( leds_plus_safety_pixel + 1);

//ModbusIP object
ModbusIP mb;

uint32_t test1[8] = {0xffffff, 0xfffdfd, 0xffa3a3, 
0xff2929, 0xff2929, 0xff2929, 0xff6a6a, 0xfff7f7};

 
void setup() {
  Serial.begin(115200);
  WiFi.begin("Magnolia-WiFi", "14MaZa14!");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  mb.server();

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS ); 

  mb.addHreg(MODE_HREG, 0x0001);
  mb.addHreg(LENGTH_HREG, 0x0000);
  mb.addHreg(PIXEL_HREG, 0x0000, 16);
  mb.addCoil(100, true); //test coil
  pinMode(LED_BUILTIN, OUTPUT); //test led
}
 
void loop() {
   mb.task();
   delay(10);
bool status = mb.Coil(100);
bool status1 = true;
    digitalWrite(LED_BUILTIN, status);

    for (int i = 0; i < NUM_LEDS*2; i++)
    {
        //leds[i] = ((mb.Hreg(PIXEL_HREG + i*2)) << 7 | (mb.Hreg(PIXEL_HREG + i*2 + 1)));
        leds[i] = test1[i];
    }
    FastLED.show();

}

/*uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
}*/
