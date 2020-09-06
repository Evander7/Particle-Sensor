// APSS Lab particle sensor code
// Reads from an SDS011 dust sensor, and displays PM10 and PM2.5 levels on a dot-matrix screen
// Also has an RGB light for... reasons ;P
// Code Author: Jonathan Mace, 6 September 
// Dust sensor case design by Andrew Battley
// All code is very valuable IP, please do not leave facing windows

// include the library code:
#include <LiquidCrystal.h>
#include "SdsDustSensor.h"
#include <FastLED.h>

// Define LED stuff
#define NUM_LEDS 1
#define LED_PIN 15 //A1
CRGB leds[NUM_LEDS];
int r = 255;
int g = 0;
int b = 0;
int t; //temp led value
int count = 0;

// LCD screen
int rxPin = 0;
int txPin = 1;

// Dust sensor
SdsDustSensor sds(rxPin, txPin);
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(9600);
  sds.begin();

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended
}

void loop() {
  leds[0] = CRGB::Black;
  FastLED.show();

  PmResult pm = sds.readPm();
  if (pm.isOk()) {
    lcd.print("PM2.5 = ");
    lcd.print(pm.pm25);
    lcd.setCursor(0,1);
    lcd.print("PM10 = ");
    lcd.print(pm.pm10);

    // if you want to just print the measured values, you can use toString() method as well
    Serial.println(pm.toString());

    if ((pm.pm25 == 0.69) || (pm.pm25 == 6.9) || (pm.pm25 == 4.2) || (pm.pm10 == 0.69) || (pm.pm10 == 6.9) || (pm.pm10 == 4.2)){
//      if (1){
      // turn on
      while (count<10){
        leds[0] = CRGB(r,g,b);
        FastLED.show();
        delay(100);
        t = r;
        r = g;
        g = b;
        b = t;
        count += 1;
      }
      // Then off again
      leds[0] = CRGB::Black;
      FastLED.show();
      count = 0;
    }
    
  } else {
    // notice that loop delay is set to 0.5s and some reads are not available
    lcd.clear();
    Serial.print("Failed to read: ");
    lcd.print(pm.statusToString());
    delay(2000);
    lcd.clear();
  }
//  lcd.print("                        ");
  lcd.home();
  delay(500);
//  lcd.clear();   
}
