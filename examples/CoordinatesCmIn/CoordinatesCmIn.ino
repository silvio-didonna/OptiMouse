// This example reads out the PixArt PAN3101 Optical Navigation Sensor
// It's used in many cheap optical mouses.
//
// Only for PAN3101
//
// The Arduino will keep track of a (x, y) coordinate by increasing
// or decreasing the x and y variables by dx and respectively dy.
// Every 300ms it reports the current (x, y) over the Serial.
//
// Written by Silvio Didonna
// Based on the sketches from: http://www.martijnthe.nl/optimouse/

#include "PAN3101.h"

#define SCLK 2                            // Serial clock pin on the Arduino
#define SDIO 3                            // Serial data (I/O) pin on the Arduino

PAN3101 Optical1 = PAN3101(SCLK, SDIO);   // Create an instance of the PAN3101 object

signed long x = 0;                        // Variables for our 'cursor'
signed long y = 0;                        

//float xin = 0;                        // Variables for our 'cursor' inch
//float yin = 0;                        
float xcm = 0;                        // Variables for our 'cursor' cm
float ycm = 0;                        

unsigned long mill;
unsigned long timePrint;
bool forceAwake = false;
bool flagManSleepDone = false; 

void setup()
{
  Serial.begin(38400);
  Optical1.begin();                       // Resync (not really necessary?)
  Optical1.force_awake_mode(forceAwake); // off
  mill = millis();
  timePrint = mill;
}

void loop()
{
  if (millis()-mill > 10000) {             
    mill = millis();
    forceAwake = !forceAwake;
    Optical1.force_awake_mode(forceAwake);         // autosleep on/off cycle 10 sec
    
    Serial.print("ForceAwake: ");
    Serial.print(forceAwake ? "true" : "false");
    Serial.println();
    
    if (forceAwake) {
       flagManSleepDone=false;
       Optical1.toggle_awake_sleep(true);         // manual wake up
    }
    
  }
  else if (flagManSleepDone ==false && forceAwake==true && millis()-mill > 3000) {              // after 3 sec in force awake mode enter sleep (force awake mode still on). forceAwake==true not really needed.
    flagManSleepDone = true;
    Optical1.toggle_awake_sleep(false);         // manual enter sleep mode
    Serial.println("Manual sleep mode on");
  }

  //  Status commands 
  //Optical1.updateStatus();                // Get the latest motion status
  //if (Optical1.motion())                  // If the 'Motion' status bit is set,
  //{

    x += Optical1.dx();                   // Read the dX register and in/decrease X with that value
    y += Optical1.dy();                   // Same thing for dY register.....
    /*
    xin=(float)x/800;
    yin=(float)y/800;
    xcm=xin*2.54;
    ycm=yin*2.54;
    */
    //xcm=x*0.003175; //800dpi
    //ycm=y*0.003175;
    xcm=x*0.00254; //1000dpi Strange behaviour, sensor resolution is 800 cpi but measured distances reflect one of 1000 cpi
    ycm=y*0.00254;
  //}
  
  if (millis() - timePrint >= 300)        // every 0.3s
  {                                       // Report the coordinates once in a while...
    /*
    Serial.print("x=");
    Serial.print(x, DEC);
    Serial.print(" y=");
    Serial.print(y, DEC);
    Serial.println();
    
    Serial.print("xin=");
    Serial.print(xin, DEC);
    Serial.print(" yin=");
    Serial.print(yin, DEC);
    Serial.println();
    */
    Serial.print("xcm=");
    Serial.print(xcm, DEC);
    Serial.print(" ycm=");
    Serial.print(ycm, DEC);
    Serial.println();

    timePrint = millis();
  }
}

