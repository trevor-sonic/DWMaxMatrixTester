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
#include "CharDefinitions.h"
#include "MaxMatrix.h"
#include "avr/pgmspace.h"


#define load  11  // CS  pin of MAX7219 module
#define clock 12  // CLK pin of MAX7219 module
#define data  13  // DIN pin of MAX7219 module
#define displayQty  1  // Display quantity
// Dots buffer
byte colBuffer[ 16 + displayQty * 8];

// Instance of module
DotDisplay m(load, clock, data, displayQty, colBuffer);

void setup()
{
//Serial.begin(9600);
//Serial.println('Start.');
//delay(100);
	m.setupDisplay          (CharSet, &systemTextHandler);
	//m.printString  ("Test");
	//m.printStringWithShift("1234567890\0", 80);

        m.printShift("1234567890\0", 500);
//test();          
}
void test()
{
  m.setColumn(8,CharSet[ 2 + 7 * 16 ]);
  m.shiftLeft(false,false);

  m.setColumn(8,CharSet[ 3 + 7 * 16 ]);
  m.shiftLeft(false,false);  
  
  m.setColumn(8,CharSet[ 4 + 7 * 16 ]);
  m.shiftLeft(false,false);

  m.setColumn(8,CharSet[ 5 + 7 * 16 ]);
  m.shiftLeft(false,false);  
}
void loop()
{
  m.engine();
}
void systemTextHandler(uint8_t txtID, char *bf)
{
//	strcpy_P(bf, (char *) pgm_read_word ( &(SysTexts[txtID]) ) );
//    myDisplay.printStringWithShift(bf, 50);
}



