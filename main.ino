#include "Arduino.h"
#include "esp.h"
#include <ESP8266WiFi.h>
#include <ModbusIP_ESP8266.h>
#include <FastLED.h>
#include <Ticker.h>
#include "font5x8.h"
#include "images.h"
//#include <WiFi_login.h>

#define LED_PIN  15
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define BRIGHTNESS 32

// Modbus registers offsets
const int MODE_HREG = 1;
const int LENGTH_HREG = 2;
const int PIXEL_HREG = 10;
// Params for width and height
const uint8_t MatrixWidth = 10;
const uint8_t MatrixHeight = 10;

#define NUM_LEDS (MatrixWidth * MatrixHeight)
CRGB leds[NUM_LEDS];
//CRGB* const leds( leds_plus_safety_pixel + 1);

//obiekt Modbusa TCP/IP
ModbusIP mb;
//obiekt Tickera -- symulacja przerwań
Ticker LEDdisp; 

uint32_t test1[8] = {0xfffff0, 0xfffdfd, 0xffa3a3, 
0xff2929, 0xff2929, 0xff2929, 0x006a00, 0x0000fa};
uint32_t tempLED[NUM_LEDS];
bool dispRdy = false;

void dispLED (){
  for (int i = 0; i < NUM_LEDS; i++)
    {
      tempLED[i] = ((mb.Hreg(PIXEL_HREG + i*2 )) << 16 | (mb.Hreg(PIXEL_HREG + i*2+1)));
        Serial.println(String("LED nr:  ") + String(i) + String(":   ") + String(tempLED[i], HEX));   //Debuging
        leds[i] = tempLED[i];
        //leds[i] = test1[i];
    }
FastLED.show();
}

void dispTEXT (){






}

/////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  //WiFi.begin("Magnolia-WiFi", "14MaZa14!");
  WiFi.begin("inz_MatrixLED", "dyplomowa");
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
  //FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS ); 

  mb.addHreg(MODE_HREG, 0x0001);
  mb.addHreg(LENGTH_HREG, 0x0000);
  mb.addHreg(PIXEL_HREG, 0x00ff, NUM_LEDS*2); //przesyłanie liczby 32 bit wymaga połączenia 2 rejestrów 16 bit
  mb.addCoil(100, true); //jedna cewka do testowania polaczenie ze SCADA
  pinMode(LED_BUILTIN, OUTPUT); //testowa dioda do sprawdzania połączenia ze SCADA
  LEDdisp.attach(1, dispLED);  //wywoływanie funkcji cyklicznie co 1 sekundę
}
 
void loop() {
   mb.task();
   delay(10);
    digitalWrite(LED_BUILTIN, mb.Coil(100));
 //     for (int i = 0; i < NUM_LEDS; i++){
  //        leds[i] = test2[i];
  //        FastLED.show();
  //    }
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
