#ifndef DotDisplay_h
#define DotDisplay_h
#include "DWMaxMatrix.h"


//Cfgs
#define chBufferSize            8
#define displayQty              1
#define mainBufferSize          80
//Defs
#define columnQty   (displayQty*8)



class DotDisplay : public DWMaxMatrix
{

public:
	DotDisplay                  ( byte load, byte clock, byte data, byte num, byte * colBuffer)  ;

        void engine();
	void setupDisplay           (const char *charSet);

	void setCharSet             (const char *cs);
	void printString            (char *s);
        void printShift             (char * s, uint8_t shift_speed);
        void                       slideLeft();
        
        
        
private:
        byte              _colIndex;
        byte             _colQty;
        char             _currChar;
        byte             _currCharIndex;
        char             _lastChar;
        char              * _currString;
        void             charShift(char  c);

        void             getChar();


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

};

#endif


