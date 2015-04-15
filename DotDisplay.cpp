// Library header
#include "DotDisplay.h"

#define writeModeString  0
#define writeModeText    1

// Code
DotDisplay::DotDisplay(byte load, byte clock, byte data, byte num, byte * colBuffer) : DWMaxMatrix(load,  clock, data,  num, colBuffer)

{
  _currString  =  "";
  _lastChar    =  0;
  _charLength  =  0;
  _delay       =  100;
  _lastMillis  =  millis();
  _running     =  false;
  _currWriteMode= writeModeString;

}
void DotDisplay::setupDisplay( const char *charSet)
{
  init                    ();         // module initialize
  setIntensity            (0);        // dot matix intensity 0-15
  _CharSet            =   charSet;

}
void DotDisplay::engine()
{

  if (_running && millis() > _lastMillis +  _delay   )
  {
    _lastMillis	=	millis();
    slideLeft();
  }

}
void DotDisplay::startShift()
{
  _currCharIndex  =  0;
  _running     =  true;
  _lastMillis  =  millis();

  getChar();  
}
void DotDisplay::textShift (const char *txt, uint8_t delayMs, functionPointer theFunction)
{
  _currWriteMode  =  writeModeText;
  _currText       =  txt;
  
  _callBackFunction=theFunction;
  _delay       =  delayMs;
  //------
  unsigned long t1= micros(); 
  _charLength  =  getTxtLen(_currText);
  unsigned long t2= micros(); 
  Serial.print("uS:"); 
  Serial.println(t2 - t1, DEC);  
  Serial.print("LEN:");   
  Serial.println(_charLength, DEC); 
 //------   
  startShift();  
}
void DotDisplay::stringShift  (char * s, uint8_t delayMs, functionPointer theFunction)
{
  _currWriteMode  =  writeModeString;
  _currString     =  s;
  
  _callBackFunction=theFunction;
  _delay       =  delayMs;
  //------
  unsigned long t1= micros(); 
  _charLength  =  getStrLen(_currString);
  unsigned long t2= micros(); 
  Serial.print("uS:"); 
  Serial.println(t2 - t1, DEC);  
  Serial.print("LEN:");   
  Serial.println(_charLength, DEC); 
 //------ 
  startShift();
}
void DotDisplay::getChar()
{

  if(_currWriteMode  ==  writeModeString)
  {
    _currChar       =  *_currString;
  }
  else if(_currWriteMode  ==  writeModeText)
  {
    _currChar       =  pgm_read_byte_near(_currText + _currCharIndex);
  }
//    Serial.print(_currCharIndex, DEC);
//    Serial.print(":");
//    Serial.println(_currChar);
  if (_currChar == 0)
  {
    //Serial.println("*END*");
    _running = false;
    if(_callBackFunction)_callBackFunction();
  }
  if (_currChar < 32) return;

  memcpy_P(buffer, _CharSet + 7 * (_currChar - 32), 7);

  //writeSprite in out of range
  writeSprite(columnQty, 0, buffer);

  //	move cursor to char	with and pur 1 column space
  setColumn(columnQty + buffer[0], 0);


  _colQty      =  buffer[0];
  _colIndex    =  2;

}
void DotDisplay::slideLeft()
{

  _colIndex++;
  shiftLeft(false, false);

  if (_colIndex > _colQty + 2)
  {
    _currCharIndex++;
    _currString++;
    getChar();
  }
}
void DotDisplay::printString(char *s)
{
  int col = 0;
  while (*s != 0)
  {
    if (*s < 32) continue;
    char c = *s - 32;
    memcpy_P(buffer, _CharSet + 7 * c, 7);
    writeSprite(col, 0, buffer);
    setColumn(col + buffer[0], 0);
    col += buffer[0] + 1;
    s++;
  }
}
byte DotDisplay::getStrLen( char *s )
{
  byte cnt = 0;
  while(*s != 0)
  {
    cnt++;
    s++;
  }
  return cnt;
}
byte DotDisplay::getTxtLen( const char *t )
{
  byte cnt =  0;
  char c   =  pgm_read_byte_near(t + cnt);
  
  while(c != 0)
  {
    cnt++;
    c   =  pgm_read_byte_near(t + cnt);
  };
  return cnt;
}
void DotDisplay::setDelay (uint8_t delayMs)
{
  _delay  =  delayMs;
}
uint8_t DotDisplay::getCharIndex()
{
  return _currCharIndex;
}
byte DotDisplay::getLength()
{
  return _charLength;
}
