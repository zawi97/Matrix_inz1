#include <Arduino.h>
#include "esp.h"
#include <Ticker.h>
#include "font5x8.h"
#include "images.h"

///////////////////////////MODBUS//////////////////////////////////
#include <ESP8266WiFi.h>
#define MB_PORT 502 
#define LED_PIN 5

#define maxInputRegister    1
#define maxHoldingRegister    210

#define MB_FC_NONE 0
#define MB_FC_READ_COILS 1
#define MB_FC_READ_DISCRETE_INPUT 2
#define MB_FC_READ_REGISTERS 3              //implemented
#define MB_FC_READ_INPUT_REGISTERS 4        //implemented
#define MB_FC_WRITE_COIL 5
#define MB_FC_WRITE_REGISTER 6              //implemented
#define MB_FC_WRITE_MULTIPLE_COILS 15
#define MB_FC_WRITE_MULTIPLE_REGISTERS 16   //implemented
//
// MODBUS Error Codes
//
#define MB_EC_NONE 0
#define MB_EC_ILLEGAL_FUNCTION 1
#define MB_EC_ILLEGAL_DATA_ADDRESS 2
#define MB_EC_ILLEGAL_DATA_VALUE 3
#define MB_EC_SLAVE_DEVICE_FAILURE 4
//
// MODBUS MBAP offsets
//
#define MB_TCP_TID          0
#define MB_TCP_PID          2
#define MB_TCP_LEN          4
#define MB_TCP_UID          6
#define MB_TCP_FUNC         7
#define MB_TCP_REGISTER_START         8
#define MB_TCP_REGISTER_NUMBER         10


class MB_TCP
{
public:
  MB_TCP(void);
    void begin(const char *ssid, const char *key);
    void Run();
    unsigned int  MBInputRegister[maxInputRegister];
    unsigned int  MBHoldingRegister[maxHoldingRegister];

private: 
    byte ByteArray[260];
    bool ledPinStatus = LOW;
    WiFiServer MBServer;
};


MB_TCP::MB_TCP(void):
    MBServer(MB_PORT)
{
}


void MB_TCP::begin(const char *ssid, const char *key){
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    ledPinStatus = LOW;
    WiFi.begin(ssid, key);
    
    while (WiFi.status() != WL_CONNECTED) {
        // Blink the LED
        digitalWrite(LED_PIN, ledPinStatus); // Write LED high/low
        ledPinStatus = (ledPinStatus == HIGH) ? LOW : HIGH;
        delay(100);
    }
    // Start the server
    digitalWrite(LED_PIN, HIGH);
    MBServer.begin();
}


void MB_TCP::Run(){
    boolean flagClientConnected = 0;
    byte byteFN = MB_FC_NONE;
    int Start;
    int WordDataLength;
    int ByteDataLength;
    int MessageLength;
    
    //****************** Read from socket ****************

        WiFiClient client = MBServer.available();
    
    while (client.connected()) {
    if(client.available())
    {
        flagClientConnected = 1;
        int i = 0;
        while(client.available())
        {
            ByteArray[i] = client.read();
            i++;
        }
            client.flush();
 
        byteFN = ByteArray[MB_TCP_FUNC];
        Start = word(ByteArray[MB_TCP_REGISTER_START],ByteArray[MB_TCP_REGISTER_START+1]);
        WordDataLength = word(ByteArray[MB_TCP_REGISTER_NUMBER],ByteArray[MB_TCP_REGISTER_NUMBER+1]);
    }
    
    // Handle request

    switch(byteFN) {
        case MB_FC_NONE:
            break;
        
        case MB_FC_READ_REGISTERS:  // 03 Read Holding Registers
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            ByteArray[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
            for(int i = 0; i < WordDataLength; i++)
            {
                ByteArray[ 9 + i * 2] = highByte(MBHoldingRegister[Start + i]);
                ByteArray[10 + i * 2] =  lowByte(MBHoldingRegister[Start + i]);
            }
            MessageLength = ByteDataLength + 9;
            client.write((const uint8_t *)ByteArray,MessageLength);
            byteFN = MB_FC_NONE;
            break;
            
        case MB_FC_READ_INPUT_REGISTERS:  // 04 Read Input Registers
            Start = word(ByteArray[MB_TCP_REGISTER_START],ByteArray[MB_TCP_REGISTER_START+1]);
            WordDataLength = word(ByteArray[MB_TCP_REGISTER_NUMBER],ByteArray[MB_TCP_REGISTER_NUMBER+1]);
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            ByteArray[8] = ByteDataLength;     //Number of bytes after this one (or number of bytes of data).
            for(int i = 0; i < WordDataLength; i++)
            {
                ByteArray[ 9 + i * 2] = highByte(MBInputRegister[Start + i]);
                ByteArray[10 + i * 2] =  lowByte(MBInputRegister[Start + i]);
            }
            MessageLength = ByteDataLength + 9;
            client.write((const uint8_t *)ByteArray,MessageLength);
            byteFN = MB_FC_NONE;
            break;
            
        case MB_FC_WRITE_REGISTER:  // 06 Write Holding Register
            MBHoldingRegister[Start] = word(ByteArray[MB_TCP_REGISTER_NUMBER],ByteArray[MB_TCP_REGISTER_NUMBER+1]);
            ByteArray[5] = 6; //Number of bytes after this one.
            MessageLength = 12;
            client.write((const uint8_t *)ByteArray,MessageLength);
            byteFN = MB_FC_NONE;
            break;
            
        case MB_FC_WRITE_MULTIPLE_REGISTERS:    //16 Write Holding Registers
            ByteDataLength = WordDataLength * 2;
            ByteArray[5] = ByteDataLength + 3; //Number of bytes after this one.
            for(int i = 0; i < WordDataLength; i++)
            {
                MBHoldingRegister[Start + i] =  word(ByteArray[ 13 + i * 2],ByteArray[14 + i * 2]);
            }
            MessageLength = 12;
            client.write((const uint8_t *)ByteArray,MessageLength);
            byteFN = MB_FC_NONE;
            break;
    }
    }
    client.stop();
    
    if (flagClientConnected == 1){
        flagClientConnected = 0;
    }
}

///////////////////////////FASTLed/////////////////////////////////

#include <FastLED.h>
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

//Wywyłanie konstruktora obiektu MB_TCP
MB_TCP Mb;
//Wywołanie konstruktora Tickera -- symulacja przerwań
Ticker LEDdisp; 

uint32_t tempLED[NUM_LEDS];
bool dispRdy = false;
uint16_t dispMode = 0;

/////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);  
  Mb.begin("Magnolia-WiFi", "14MaZa14!");

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS ); 

  LEDdisp.attach(1, dispLED);  //wywoływanie funkcji cyklicznie co 1 sekundę
}
 
void loop() {
  Mb.Run();
  delay(10);

   digitalWrite(LED_BUILTIN, false);
dispMode = Mb.MBHoldingRegister[MODE_HREG];

switch (dispMode)
{
case 1:
    MBtoLED();
  break;
case 2:
    dispTEXT(Mb.MBHoldingRegister[10]);
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
        tempLED[i] = (Mb.MBHoldingRegister[DATA_HREG + i*2 ] << 16 | Mb.MBHoldingRegister[DATA_HREG + i*2+1]);
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

uint16_t XY(uint8_t x, uint8_t y)
{
  uint16_t i;
    i = (y * MatrixWidth) + x;
  return i;
}

uint16_t XY_s(uint8_t x, uint8_t y)
{
  if(x >= MatrixWidth) return -1;
  if(y >= MatrixHeight) return -1;
  return XY(x, y);
}