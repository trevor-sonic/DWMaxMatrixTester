#ifndef DotDisplay_h
#define DotDisplay_h
#include "DWMaxMatrix.h"
#include <avr/pgmspace.h>

//Definitions
#define columnQty               (displayQty*8)
#define colBufferQty            (columnQty + 8)  //1 empty virtual display buffer on the  right
#define writeModeString  0
#define writeModeText    1
#define slideDirectionLeft        0
#define slideDirectionRight       1

//#include "Arduino.h"
//-----------------------------------------------
typedef void (*functionPointer)(void);
//-----------------------------------------------


class DotDisplay : public DWMaxMatrix
{

public:
	DotDisplay                  ( byte load, byte clock, byte data, byte num, byte * colBuffer)  ;

        void engine();
	void setupDisplay           (const char *charSet);

	void setCharSet             (const char *cs);
        void setDelay               (uint8_t delayMs);
        byte getDelay               ();
        
        uint8_t getCharIndex        ();
        byte                        getLength();  
        
	void printString            (char *s);

        void  stringShift            (char * s, uint8_t delayMs, functionPointer theFunction);
        void  textShift              (const char *txt, uint8_t delayMs, functionPointer theFunction);
        void  setDirection           (byte d);
        
        
        
        
private:
        byte             _colIndex;
        byte             _colQty;
        char             _currChar;
        byte             _currCharIndex;

        char              * _currString;
        const char        * _currText;
        byte              _currWriteMode;
        
        byte             _charLength;
        
        void             charShift(char  c);

        void             getChar();

        byte             _slideDirection;
        void             startShift();
        void             slide();
        void             slideLeft();
        void             slideRight();
        
        
        byte             getStrLen( char *s );
        byte             getTxtLen( const char *t );
        
	byte                temp[8];// 1 char is 8 bytes.
	int                 diplayOffset;
	
	const char        * _CharSet;
	const char        * _texts;
	const char        * _textLens;

	bool			_running;
	bool			_currStatus;

	unsigned long 	_lastMillis;
	uint8_t  	_delay;
        
        void        (*_callBackFunction)    (void);
};

#endif


