


#include "Arduino.h"


#ifndef DotDisplay_h
#define DotDisplay_h
#include "DWMaxMatrix.h"
//#include <avr/pgmspace.h>

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
	void setupDisplay           (const char *charSet, void(*fptr)(uint8_t, char * ) );
	void printStrInt            (String s, int i);
	void printStrInt2           (String s, int i, String s2, int i2);
	void setCharSet             (const char *cs);
	void printString            (char *s);
        
        void printShift             (char * s, uint8_t shift_speed);
        
	void printStringWithShift   (char * s, uint8_t shift_speed);
	void printStringWithShift2  (String s, uint8_t shift_speed);
	void printCharWithShift     (char c, uint8_t shift_speed);
	void moveLeft               (uint8_t move);
	void moveRight              (uint8_t move);
	void moveUp                 (uint8_t move);
	void moveDown               (uint8_t move);

	void printNumbers           (String s, uint8_t *arr, uint8_t len, uint8_t speed);

	void systemText             (uint8_t txtID);
	void systemTextInt          (uint8_t txtID, int i);
	void systemTextWithShift    (uint8_t txtID, uint8_t speed);
	void systemTextWithShift2   (uint8_t txtID, String s, uint8_t speed);

	void (*_systemTextHandler)  (uint8_t txtID, char *);
        void               slideLeft();
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


