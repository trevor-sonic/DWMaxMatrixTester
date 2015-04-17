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
#include "Texts.h"
#include "DWEasyInterval.h"



#define load  11  // CS  pin of MAX7219 module
#define clock 12  // CLK pin of MAX7219 module
#define data  13  // DIN pin of MAX7219 module
#define displayQty  1  // Display quantity

// Dots buffer ** do not modify below 
byte colBuffer[ colBufferQty ];

// Instance of module
DotDisplay m(load, clock, data, displayQty, colBuffer);
DWEasyInterval interval;

void setup()
{
  Serial.begin(9600);
  Serial.println("Start.");



  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);


  m.setupDisplay          (CharSet);
  //m.textShift(TEXT01, 100, printEnd);

  m.setDirection(slideDirectionRight);
  m.stringShift("0123456789ABCDEF", 200, printEnd);
  
  interval.setFunction(changeDir);
  interval.setDelay(100, 0);
  //interval.start();

}

void loop()
{
  m.engine();
  interval.loop();
}
void changeDir()
{
  if(m.getCharIndex()>5)
  {
    //delay(1000);
    m.setDirection(slideDirectionRight);
  }
  else if(m.getCharIndex()<1)
  {
    //delay(1000);
    m.setDirection(slideDirectionLeft);
  }
  
}
void printEnd()
{
  Serial.println("-> printEnd");
  Serial.print("char len was:");
  Serial.println(m.getLength(), DEC);
  //m.stringShift("0123456789", 20, printEnd);
  
}
