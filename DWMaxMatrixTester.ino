/*
 ****************************************************************************
 *
 * DWMaxMatrixTester is a simple code to show DWMaxMatrix library functionalities
 * Created by Trevor D. BEYDAG
 * Version 1.01 Avril 2015
 * Source:
 * Code license GNU GPL v3 http://www.gnu.org/licenses/gpl.html
 *
 ****************************************************************************
 */
#include "DotDisplay.h"
#include "CharDefinitions.h"
#include "DWEasyInterval.h"



#define load  11  // CS  pin of MAX7219 module
#define clock 12  // CLK pin of MAX7219 module
#define data  13  // DIN pin of MAX7219 module
#define displayQty  1  // Display quantity
// Dots buffer
byte colBuffer[ 16 + displayQty * 8];

// Instance of module
DotDisplay m(load, clock, data, displayQty, colBuffer);
DWEasyInterval interval;

void setup()
{
  Serial.begin(9600);
  Serial.print('Start.');



  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);


  m.setupDisplay          (CharSet);
  m.printShift("abc!.123", 90);

interval.setFunction(refresh);
interval.setDelay(500,0);
interval.start();

}

void loop()
{
  m.engine();
  interval.loop();
}
void refresh()
{
  Serial.println(millis(), DEC); 
}

