#include "Arduino.h"
#include "esp.h"
#include <ESP8266WiFi.h>
#include <ModbusIP_ESP8266.h>
#include <FastLED.h>
#include <Ticker.h>
#include <pgmspace.h>
#include "font5x8.h"
#include "images.h"


#define LED_PIN  15
#define COLOR_ORDER GRB
#define CHIPSET     WS2812
#define BRIGHTNESS 32

// Offset rejestrów Modbusa
const int TEST_COIL = 100;
const int MODE_HREG = 1;
const int TEXT_HREG = 2;
const int DATA_HREG = 10;

//obiekt Modbusa TCP/IP
ModbusIP mb;
//obiekt Tickera -- symulacja przerwań
Ticker timerDisp; 
Ticker timerText;
Ticker timerTextPict;
Ticker timerAnimation;

// Rozmiary matrycy
const uint8_t MatrixWidth = 10;
const uint8_t MatrixHeight = 10;

#define NUM_LEDS (MatrixWidth * MatrixHeight)
//CRGB leds[NUM_LEDS];
CRGB leds_s[NUM_LEDS + 1];
CRGB * const leds(leds_s + 1);
uint16_t XY(uint8_t, uint8_t);
uint16_t XY_s(uint8_t, uint8_t);
uint16_t checkLetter(uint16_t);

void MBtoLED (void);
void dispTEXT (uint16_t);
void dispTwoTEXT (void);
void dispTwoTEXTPict (void);
void dispLED (void);
void dispClear(void);
void dispModeChange (uint16_t, uint16_t);

uint32_t tempLED[NUM_LEDS]; //bufor wyświetlanych pikseli 
bool dispRdy = false;       //nieużywane
uint16_t dispMode = 0;      //tryb działania i wyświetlania na matrycy 1 - obraz przesyłany przez MODBUSA
                            //2 - wyświetlanie znaków
uint16_t lastDispMode = 0;
short itter = 0;
/////////////////////////////////////////////////////////////////////////////////


void setup() {
  Serial.begin(115200);
  WiFi.begin("inz_MatrixLED", "dyplomowa");
  //WiFi.begin("Magnolia-WiFi", "14MaZa14!");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //Wyświetla adres IP urządzenia w sieci 

  mb.server();

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS ); 

  mb.addHreg(MODE_HREG, 0x0004); 
  mb.addHreg(TEXT_HREG, 0x0000, 2);
  mb.addHreg(DATA_HREG, 0x00ff, NUM_LEDS*2); //przesyłanie liczby 32 bit wymaga połączenia 2 rejestrów 16 bit
  mb.addCoil(100, true); //jedna cewka do testowania polaczenie ze SCADA
  pinMode(LED_BUILTIN, OUTPUT); //testowa dioda do sprawdzania połączenia ze SCADA
  timerDisp.attach(0.2, dispLED);  //wywoływanie funkcji cyklicznie co 1 sekundę
  timerText.attach(2, dispTwoTEXT);
  timerTextPict.attach(2, dispTwoTEXTPict);
}
 
void loop() {
   mb.task();
   delay(10);
   digitalWrite(LED_BUILTIN, mb.Coil(TEST_COIL)); //testowe zapalanie diody z systemu SCADA 
  dispMode = mb.Hreg(MODE_HREG);
  //dispModeChange(dispMode, lastDispMode);
  lastDispMode = dispMode;

switch (dispMode)
{
case 1:
//Wyświetlanie obrazu przesłąnego przez SCADA
    MBtoLED();
  break;
case 2:
//Wyświetlanie jednego znaku przesłąnego przez SCADA
    dispTEXT(mb.Hreg(DATA_HREG));
  break;
case 3:
//wyświetlanie dwóch znaków naprzemian wysłanych przez SCADA
break;
case 4:
//wyświetlanie dwóch znaków oraz grafiki naprzemian wysłanych przez SCADA
break;
case 5:
    for (int i = 0; i < NUM_LEDS; i++)
        {
            tempLED[i] = pgm_read_dword(&(fix_16[i]));
        }
break;
case 6:
    for (int i = 0; i < NUM_LEDS; i++)
        {
            tempLED[i] = pgm_read_dword(&(battery_10[i]));
        }
break;
case 7:
    for (int i = 0; i < NUM_LEDS; i++)
        {
            tempLED[i] = pgm_read_dword(&(human_8[i]));
        }
break;
case 8:
    for (int i = 0; i < NUM_LEDS; i++)
        {
            tempLED[i] = pgm_read_dword(&(ok_17G[i]));
        }
break;
case 9:
    for (int i = 0; i < NUM_LEDS; i++)
        {
            tempLED[i] = pgm_read_dword(&(ok_17[i]));
        }
break;
case 10:
    for (int i = 0; i < NUM_LEDS; i++)
        {
            tempLED[i] = pgm_read_dword(&(thunder_6[i]));
        }
break;
case 11:
    for (int i = 0; i < NUM_LEDS; i++)
        {
            tempLED[i] = pgm_read_dword(&(network_4[i]));
        }
break;
case 12:
    for (int i = 0; i < NUM_LEDS; i++)
        {
            tempLED[i] = pgm_read_dword(&(plug_14[i]));
        }
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

void dispTEXT (uint16_t letter){
  uint16_t ascii_char = checkLetter(letter); //sprawdzenie, czy indeks liter nie wychodzi poza zakres
  dispClear();
  for (uint8_t y = 0; y < 8; y++)
  {
    for (uint8_t x = 0; x < 5; x++)
    {
      if (fontHEX[ascii_char][x] & (1<<y))   tempLED[XY_s(x+2, y+1)] = CRGB::Red;
      else                                   tempLED[XY_s(x+2, y+1)] = CRGB::Black;
    }
  }
}

void dispTwoTEXT (void){
    if(dispMode == 3){
    uint16_t letters[] = {(int)mb.Hreg(TEXT_HREG), (int)mb.Hreg(TEXT_HREG + 1)};
     dispTEXT(letters[itter%2]);
    
    itter++;
    }
}

void dispTwoTEXTPict (void){
    if(dispMode == 4){
      if(itter%4 == 0 || itter%4 == 1) MBtoLED();
      else if (itter%4 == 2 || itter%4 == 3)
      {
        uint16_t letters[] = {(int)mb.Hreg(TEXT_HREG), (int)mb.Hreg(TEXT_HREG + 1)};
        dispTEXT(letters[itter%2]);
      }

    itter++;
    }
}

void dispLED (){
  for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = tempLED[99 - i];
        //tempLED[i] = CRGB::Black;
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

uint16_t checkLetter(uint16_t letter){
  if (letter >= 0 && letter < 95) return letter;
  else return 95;
}

void dispModeChange(uint16_t currentMode, uint16_t lastMode){
  if (lastMode == 3 && currentMode != lastMode)
  {
    timerText.detach();
    itter = 1;
  }
}
void dispClear(){
    for (int i = 0; i < NUM_LEDS; i++)
    {
        tempLED[i] = CRGB::Black;
    }
}