// Library header
#include "DotDisplay.h"



// Code
DotDisplay::DotDisplay(byte load, byte clock, byte data, byte num, byte * colBuffer)
  : DWMaxMatrix(load,  clock, data,  num, colBuffer)

{
  _currString  =  "";
  _charLength  =  0;
  _delay       =  100;
  _lastMillis  =  millis();
  _running     =  false;
  _currWriteMode = writeModeString;
  _slideDirection = slideDirectionLeft;
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
    slide();
  }

}
void DotDisplay::startShift()
{
  if (_slideDirection == slideDirectionLeft)
  {  
    _currCharIndex  =  0;
  }
  else if (_slideDirection == slideDirectionRight)
  {
    _currCharIndex  =  _charLength;
  }
  _running     =  true;
  _lastMillis  =  millis();

  getChar();
}
void DotDisplay::textShift (const char *txt, uint8_t delayMs, functionPointer theFunction)
{
  _currWriteMode  =  writeModeText;
  _currText       =  txt;

  _callBackFunction = theFunction;
  _delay       =  delayMs;

  _charLength  =  getTxtLen(_currText);

  startShift();
}
void DotDisplay::stringShift  (char * s, uint8_t delayMs, functionPointer theFunction)
{
  _currWriteMode  =  writeModeString;
  _currString     =  s;

  _callBackFunction = theFunction;
  _delay       =  delayMs;

  _charLength  =  getStrLen(_currString);

  startShift();
}
void DotDisplay::getChar()
{

Serial.print("_currCharIndex:");
Serial.println(_currCharIndex);

  if (_currWriteMode  ==  writeModeString)
  {
    //CORRECTION NEED!!!
    _currChar       =  *_currString;
    //memcpy( _currChar  , _currString   , 1);
    
    //_currChar  =  pgm_read_byte_near(_currString + _currCharIndex);
    //char *c =_currChar;
    //memcpy( *_currChar  , _currString   , 1);
  }
  else if (_currWriteMode  ==  writeModeText)
  {
    _currChar       =  pgm_read_byte_near(_currText + _currCharIndex);
  }
  if (_currChar == 0)
  {
    _running = false;
    if (_callBackFunction)_callBackFunction();
  }

Serial.print("_currChar:");  
Serial.println(_currChar);

  if (_currChar < 32) return;

  memcpy_P(_temp, _CharSet + 7 * (_currChar - 32), 7);

  _charColQty      =  _temp[0];
  _charArrIndex    =  2;

  insertCharColIntoBuffer();

}
void DotDisplay::insertCharColIntoBuffer()
{
  if (_slideDirection == slideDirectionLeft)
  {
    byte startColumnFrom  =  (num * 8);
  
    // writeSprite in out of range
    writeSprite(startColumnFrom, 0, _temp);
  
    // move cursor to char	with and pur 1 column space
    setColumn(startColumnFrom + _charColQty , 0);
  }
  else if (_slideDirection == slideDirectionRight)
  {
    byte startColumnFrom  =  7 - _charColQty;

    memcpy( _leftBuffer + startColumnFrom  , _temp + _charArrIndex   , _charColQty);
    
    //DEBUGER vvvvvvvvvvv
     for(byte i=0; i<7; i++)
    {
        if (_temp[i]<128) Serial.print('0');
        if (_temp[i]<64) Serial.print('0');
        if (_temp[i]<32) Serial.print('0');
        if (_temp[i]<16) Serial.print('0');
        if (_temp[i]<8) Serial.print('0');
        if (_temp[i]<4) Serial.print('0');
        if (_temp[i]<2) Serial.print('0');
      Serial.println(_temp[i], BIN);
    }
    Serial.println("------------");
    for(byte i=0; i<8; i++)
    {
        if (_leftBuffer[i]<128) Serial.print('0');
        if (_leftBuffer[i]<64) Serial.print('0');
        if (_leftBuffer[i]<32) Serial.print('0');
        if (_leftBuffer[i]<16) Serial.print('0');
        if (_leftBuffer[i]<8) Serial.print('0');
        if (_leftBuffer[i]<4) Serial.print('0');
        if (_leftBuffer[i]<2) Serial.print('0');
      Serial.println(_leftBuffer[i], BIN);
    }
    
    //^^^^^^^^^^^^^^^^^^^
  }
}
void DotDisplay::slide()
{
  if (_slideDirection == slideDirectionLeft)
  {
    slideLeft();
  }
  else if (_slideDirection == slideDirectionRight)
  {
    slideRight();
  }
}
void DotDisplay::slideLeft()
{
  _charArrIndex++;
  shiftLeft(false, false);

  if (_charArrIndex > _charColQty + 2)
  {
    _currCharIndex++;
    _currString++;
    getChar();
  }
}
void DotDisplay::slideRight()
{
  _charArrIndex++;
  shiftRight(false, false);

  if (_charArrIndex > _charColQty + 2)
  {
    _currCharIndex--;
    _currString--;
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
    memcpy_P(_temp, _CharSet + 7 * c, 7);
    writeSprite(col, 0, _temp);
    setColumn(col + _temp[0], 0);
    col += _temp[0] + 1;
    s++;
  }
}
byte DotDisplay::getStrLen( char *s )
{
  byte cnt = 0;
  while (*s != 0)
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

  while (c != 0)
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
byte DotDisplay::getDelay()
{
  return _delay;
}
void DotDisplay::setDirection(byte d)
{
  _slideDirection  =  d;
  Serial.print("_slideDirection is set to:");
  Serial.println(_slideDirection, DEC);
}
