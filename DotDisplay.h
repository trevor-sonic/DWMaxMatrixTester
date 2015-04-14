#ifndef DotDisplay_h
#define DotDisplay_h
#include "DWMaxMatrix.h"
#include <avr/pgmspace.h>

//Cfgs
#define chBufferSize            8
#define displayQty              1
#define mainBufferSize          80
//Defs
#define columnQty   (displayQty*8)


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
        uint8_t getCharIndex        ();
        
	void printString            (char *s);
        void stringShift            (char * s, uint8_t delayMs, functionPointer theFunction);
        void textShift              (const char *txt, uint8_t delayMs, functionPointer theFunction);
        
        void                       slideLeft();
        
        
        
private:
        byte              _colIndex;
        byte             _colQty;
        char             _currChar;
        byte             _currCharIndex;
        char             _lastChar;
        char              * _currString;
        const char        * _currText;
        byte              _currWriteMode;
        
        void             charShift(char  c);

        void             getChar();

        void             startShift();

	byte                buffer[8];// 1 char is 8 bytes.
	int                 diplayOffset;
	char                _charBuf            [chBufferSize];
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


