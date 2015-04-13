// Library header
#include "DotDisplay.h"

// Code
DotDisplay::DotDisplay(byte load, byte clock, byte data, byte num, byte * colBuffer) : DWMaxMatrix(load,  clock, data,  num, colBuffer)

{
  _currString="";
  _lastChar=0;

	_delay		=	100;
	_lastMillis	=	millis();
	_running	=	false;

}
void DotDisplay::setupDisplay( const char *charSet, void(*fptr)(uint8_t, char * ) )
{
  init                    ();         // module initialize
  setIntensity            (0);        // dot matix intensity 0-15
  _CharSet            =   charSet;
  _systemTextHandler  =   fptr;
}
void DotDisplay::moveLeft()
{

  /*
  if (*_currString == 0)
  {
    _lastChar   =  0;
   // _running    =  false;
    return;
  } */
    
      _currChar  = *_currString;  
      if (_currChar < 32) return;
    
      
  if(_lastChar  !=  _currChar)
  {


      
      _colQty    =  _CharSet[ 7 * (_currChar-32) ];
      _lastChar  = _currChar;
      _colIndex = 0;
  }
  
  //_currString++;


  setColumn(columnQty, _CharSet[ _colIndex + (7 * (_currChar - 32)) ]);
  _colIndex++;
  // 	writeSprite from 32th column
  //writeSprite(columnQty, 0, buffer);

  //	move cursor to char	with and pur 1 column space
  //setColumn(columnQty + buffer[0], 0);

  //	Shift left "char with + 1" times
  //for (int i = 0; i < buffer[0] + 1; i++)
  //{
   // delay(shift_speed);
    shiftLeft(false, false);
  //}  

  if(_colIndex>=_colQty)
  {
    _currString++;
  }
}
void DotDisplay::printStringWithShift(char * s, int shift_speed)
{
  while (*s != 0)
  {
    printCharWithShift(*s, shift_speed);
    s++;
  }
}
void DotDisplay::engine()
{
    if ( _running && (millis() >= _lastMillis + _delay) )
    {
    	_lastMillis	=	millis();
        moveLeft();
    }
}
void DotDisplay::printShift  (char * s, int delayMs)
{
    _currString  =  s;
    _delay       =  delayMs;
    _running     =  true;
    _lastMillis  =  millis();
}
void DotDisplay::printCharWithShift(char c, int shift_speed)
{
  // unknown char, do nothing!
  if (c < 32) return;



  // offset of char in array
  c -= 32;
  // c=0  for space
  // c=1  for !
  // ......
  // c=33 for A
  // c=34 for B
  // and so ....


  //	3, 8, B00000000, B00000000, B00000000, B00000000, B00000000, // space 	(0)
  //	1, 8, B01011111, B00000000, B00000000, B00000000, B00000000, // ! 		(1)
  //  ..........
  //	4, 8, B01111110, B00010001, B00010001, B01111110, B00000000, // A		(33)
  //	4, 8, B01111111, B01001001, B01001001, B00110110, B00000000, // B		(34)

  //	7 is quantity of value for 1 char definetion
  //	buffer[0]	=	Width of char, Qty of column, 4 for "A"
  //	buffer[1]	=	Height of char, Qty of bits, 8 allways
  //	buffer[2,3,4,5,6]	=	Binary definitions of char


  //	for "A"
  //	_CharSet (pointer) + 7 * 33 = 231
 
  // 	memcpy_P into buffer from 231,   7 bytes
  memcpy_P(buffer, _CharSet + 7 * c, 7);

  // 	writeSprite from 32th column
  writeSprite(columnQty, 0, buffer);

  //	move cursor to char	with and pur 1 column space
  setColumn(columnQty + buffer[0], 0);

  //	Shift left "char with + 1" times
  for (int i = 0; i < buffer[0] + 1; i++)
  {
   delay(shift_speed);
    shiftLeft(false, false);
  }
  
}
void DotDisplay::printString(char *s)
{
  int col = 0;
  while (*s != 0)
  {
    if (*s < 32) continue;
    char c = *s - 32;
    //        memcpy_P(buffer, CH + 7*c, 7);
    memcpy_P(buffer, _CharSet + 7 * c, 7);
    //        for (uint8_t i= 0; i<8; i++)
    //        {
    //            buffer[i]   =   EEPROM.read(i + 7*c);
    //        }

    writeSprite(col, 0, buffer);
    setColumn(col + buffer[0], 0);
    col += buffer[0] + 1;
    s++;
  }
}
void DotDisplay::printStrInt(String s, int i)
{
  String str  =  s;
  str +=  i ;
  str +=  "  ";

  str.toCharArray(_charBuf, chBufferSize);
  clear();
  printString(_charBuf);
}
void DotDisplay::systemTextInt(uint8_t txtID, int i)
{
  String s  =  "";
  s +=  i ;

  char buf[3];
  s.toCharArray(buf, 3);

  _systemTextHandler( txtID,  _charBuf);


  strcat(_charBuf, buf); /* add the integer  */

  printString(_charBuf);
}
void DotDisplay::printStrInt2(String s, int i, String s2, int i2)
{
  String str  =  "";
  str +=  s;
  str +=  i ;
  str +=  s2;
  str +=  i2;

  str.toCharArray(_charBuf, chBufferSize);
  clear();
  printString(_charBuf);
}
void DotDisplay::printStringWithShift2(String s, int shift_speed)
{
  String str  =  "";
  str += s;
  str.toCharArray(_charBuf, chBufferSize);
  printStringWithShift(_charBuf, shift_speed);
}
//void DotDisplay::systemTextWithShift(uint8_t txtID, uint8_t speed)
//{
//
//    _systemTextHandler( txtID,  speed);
//
//}
void DotDisplay::systemText(uint8_t txtID)
{
  _systemTextHandler( txtID,  _charBuf);
  printString(_charBuf);
}
void DotDisplay::systemTextWithShift(uint8_t txtID, uint8_t speed)
{
  _systemTextHandler( txtID,  _charBuf);
  printStringWithShift(_charBuf, speed);
}
void DotDisplay::systemTextWithShift2(uint8_t txtID, String s, uint8_t speed)
{
  char buf[3];
  s.toCharArray(buf, 3);

  _systemTextHandler( txtID,  _charBuf);

  strcat(_charBuf, buf); /* add the value  */

  printStringWithShift2(_charBuf, speed);
}
void DotDisplay::moveLeft(uint8_t move)
{
  for (uint8_t i = 0; i <= move; i++)
  {
    shiftLeft(true, true);
    delay(25);
  }
}
void DotDisplay::moveRight(uint8_t move)
{
  for (uint8_t i = 0; i <= move; i++)
  {
    shiftRight(true, true);
    delay(25);
  }
}
void DotDisplay::moveUp(uint8_t move)
{
  for (uint8_t i = 0; i <= move; i++)
  {
    shiftUp(true);
    delay(25);
  }
}
void DotDisplay::moveDown(uint8_t move)
{
  for (uint8_t i = 0; i <= move; i++)
  {
    shiftDown(true);
    delay(25);
  }
}
void DotDisplay::printNumbers(String s, uint8_t *arr, uint8_t len, uint8_t speed)
{

  for (uint8_t i = 0 ; i < len; i++)
  {
    s += String(arr[i], DEC);
    s += ", ";
  }

  //    char cBuf[4];
  s.toCharArray(_charBuf, 4);
  printStringWithShift( _charBuf  , speed);
}
