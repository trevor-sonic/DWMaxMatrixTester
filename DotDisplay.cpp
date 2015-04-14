// Library header
#include "DotDisplay.h"

// Code
DotDisplay::DotDisplay(byte load, byte clock, byte data, byte num, byte * colBuffer) : DWMaxMatrix(load,  clock, data,  num, colBuffer)

{
  _currString = "";
  _lastChar = 0;

  _delay		=	100;
  _lastMillis	=	millis();
  _running	=	false;

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
    digitalWrite(3,HIGH);
  }
  else
  {
  digitalWrite(3,LOW);
  return;
  }

  
}
void DotDisplay::printShift  (char * s, uint8_t delayMs)
{
  _currString     =  s;
  _currCharIndex  =  0;

  _delay       =  delayMs;
  _running     =  true;
  _lastMillis  =  millis();

  getChar();
}
void DotDisplay::getChar()
{
  _currChar       =  *_currString;
  if (_currChar != 0)
  {}
  else
  {
    _running = false;
    return;
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
