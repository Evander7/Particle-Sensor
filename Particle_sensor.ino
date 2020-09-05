
// include the library code:
#include <LiquidCrystal.h>
#include "SdsDustSensor.h"
#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 1
#define LED_PIN A1
CRGB leds[NUM_LEDS];

int rxPin = 0;
int txPin = 1;
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
  // Turn off the display:
//  lcd.noDisplay();
//  delay(500);
  // Turn on the display:
//  lcd.display();
//  delay(500);

  PmResult pm = sds.readPm();
  if (pm.isOk()) {
    lcd.print("PM2.5 = ");
    lcd.print(pm.pm25);
    lcd.setCursor(0,1);
    lcd.print("PM10 = ");
    lcd.print(pm.pm10);

    // if you want to just print the measured values, you can use toString() method as well
    Serial.println(pm.toString());
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
//  lcd.clear();
  delay(500);

/*
  // Turn the LED on, then pause
  leds[0] = CRGB(1,125,125);
  FastLED.show();
  delay(500);
  // Now turn the LED off, then pause
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
*/
  
}
