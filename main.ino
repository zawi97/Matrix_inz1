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
const int DATA_HREG = 10;
// Rozmiary matrycy
const uint8_t MatrixWidth = 10;
const uint8_t MatrixHeight = 10;

#define NUM_LEDS (MatrixWidth * MatrixHeight)
//CRGB leds[NUM_LEDS];
CRGB leds_s[NUM_LEDS + 1];
CRGB * const leds(leds_s + 1);
uint16_t XY(uint8_t, uint8_t);
uint16_t XY_s(uint8_t, uint8_t);

void MBtoLED(void);
void dispTEXT (uint16_t);
void dispLED (void);

//obiekt Modbusa TCP/IP
ModbusIP mb;
//obiekt Tickera -- symulacja przerwań
Ticker LEDdisp; 

uint32_t tempLED[NUM_LEDS]; //bufor wyświetlanych pikseli 
bool dispRdy = false;       //nieużywane
uint16_t dispMode = 0;      //tryb działania i wyświetlania na matrycy 1 - obraz przesyłany przez MODBUSA
                            //2 - wyświetlanie znaków

/////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  //WiFi.begin("Magnolia-WiFi", "14MaZa14!");
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
  //mb.addHreg(LENGTH_HREG, 0x0000);
  mb.addHreg(DATA_HREG, 0x00ff, NUM_LEDS*2); //przesyłanie liczby 32 bit wymaga połączenia 2 rejestrów 16 bit
  mb.addCoil(100, true); //jedna cewka do testowania polaczenie ze SCADA
  pinMode(LED_BUILTIN, OUTPUT); //testowa dioda do sprawdzania połączenia ze SCADA
  LEDdisp.attach(0.2, dispLED);  //wywoływanie funkcji cyklicznie co 1 sekundę
}
 
void loop() {
   mb.task();
   delay(10);
   digitalWrite(LED_BUILTIN, mb.Coil(100));
dispMode = mb.Hreg(MODE_HREG);

switch (dispMode)
{
case 1:
    MBtoLED();
  break;
case 2:
    dispTEXT(40);
  break;
case 3:
    //wyświetlanie dwóch znaków naprzemian
break;
default:
    //jakaś animacja
  break;
}

}

void MBtoLED(){
    for (int i = 0; i < NUM_LEDS; i++)
    {
        tempLED[i] = ((mb.Hreg(DATA_HREG + i*2 )) << 16 | (mb.Hreg(DATA_HREG + i*2+1)));
        Serial.println(String("LED nr:  ") + String(i) + String(":   ") + String(tempLED[i], HEX));   //Debuging
    }
}

void dispTEXT (uint16_t ascii_char){
for (uint8_t y = 0; y < 8; y++)
{
  for (uint8_t x = 0; x < 5; x++)
  {
    if (fontHEX[ascii_char][x] & (1<<y))   tempLED[XY_s(x+2, y+1)] = CRGB::Red;
    else                                   tempLED[XY_s(x+2, y+1)] = CRGB::Black;
  }
}
}

void dispLED (){
  for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = tempLED[i];
        tempLED[i] = CRGB::Black;
    }
FastLED.show();
}

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
    i = (y * MatrixWidth) + x;
  return i;
}

uint16_t XY_s( uint8_t x, uint8_t y)
{
  if( x >= MatrixWidth) return -1;
  if( y >= MatrixHeight) return -1;
  return XY(x,y);
}