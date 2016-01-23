// This example reads out the PixArt PAN3101 Optical Navigation Sensor
// It's used in many cheap optical mouses.
//
// Only for PAN3101
//
// PowerOff mode test.
//
// Written by Silvio Didonna
// Based on the sketches from: http://www.martijnthe.nl/optimouse/

#include "PAN3101.h"

#define SCLK 2                            // Serial clock pin on the Arduino
#define SDIO 3                            // Serial data (I/O) pin on the Arduino

PAN3101 Optical1 = PAN3101(SCLK, SDIO);   // Create an instance of the PAN3101 object

unsigned long mill;
bool powerDown = true;

void setup()
{
  Serial.begin(38400);
  Optical1.begin();                       // Resync (not really necessary?)
  Optical1.toggle_power_down(powerDown);
  Serial.print("PowerDownMode: ");
  Serial.print(powerDown ? "on" : "off");
  Serial.println();
  mill = millis();
}

void loop()
{
  if (millis()-mill > 10000) {             
    mill = millis();
    powerDown = !powerDown;
    Optical1.toggle_power_down(powerDown);
    Serial.print("PowerDownMode: ");
    Serial.print(powerDown ? "on" : "off");
    Serial.println();
  }
}

