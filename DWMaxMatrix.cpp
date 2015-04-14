/*
 ****************************************************************************
 *
 * MaxMatrix is created by Copyright Oscar Kin-Chung Au
 * Version 1.0 Feb 2013
 * Original source: https://code.google.com/p/arudino-maxmatrix-library/
 * Code license GNU GPL v3 http://www.gnu.org/licenses/gpl.html
 *
 ****************************************************************************
 *
 * DWMaxMatrix is slightly modified version of original MaxMatrix lib.
 * Renamed as DWMaxMatrix to prevent any conflict with exists MaxMatrix lib.
 * Modified by Trevor D. BEYDAG
 * Version 1.01 Avril 2015
 * Modified source:
 * Code license GNU GPL v3 http://www.gnu.org/licenses/gpl.html
 *
 ****************************************************************************
 */

#include "DWMaxMatrix.h"

DWMaxMatrix::DWMaxMatrix( byte _load, byte _clock, byte _data, byte _num, byte * colBuffer) 
{
	data = _data;
	load = _load;
	clock = _clock;
	num = _num;
       buffer  =  colBuffer;
	for (int i=0; i<80; i++)
		buffer[i] = 0;
}

void DWMaxMatrix::init()
{
	pinMode(data,  OUTPUT);
	pinMode(clock, OUTPUT);
	pinMode(load,  OUTPUT);
	digitalWrite(clock, HIGH); 

	setCommand(max7219_reg_scanLimit, 0x07);      
	setCommand(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
	setCommand(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	setCommand(max7219_reg_displayTest, 0x00); // no display test
	
	// empty registers, turn all LEDs off
	clear();
	
	setIntensity(0x0f);    // the first 0x0f is the value you can set
}

void DWMaxMatrix::setIntensity(byte intensity)
{
	setCommand(max7219_reg_intensity, intensity);
}

void DWMaxMatrix::clear()
{
	for (int i=0; i<8; i++) 
		setColumnAll(i,0);
		
	for (int i=0; i<80; i++)
		buffer[i] = 0;
}

void DWMaxMatrix::setCommand(byte command, byte value)
{
	digitalWrite(load, LOW);    
	for (int i=0; i<num; i++) 
	{
		shiftOut(data, clock, MSBFIRST, command);
		shiftOut(data, clock, MSBFIRST, value);
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
}


void DWMaxMatrix::setColumn(byte col, byte value)
{
	int n = col / 8;
	int c = col % 8;
	digitalWrite(load, LOW);    
	for (int i=0; i<num; i++) 
	{
		if (i == n)
		{
			shiftOut(data, clock, MSBFIRST, c + 1);
			shiftOut(data, clock, MSBFIRST, value);
		}
		else
		{
			shiftOut(data, clock, MSBFIRST, 0);
			shiftOut(data, clock, MSBFIRST, 0);
		}
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
	
	buffer[col] = value;
}

void DWMaxMatrix::setColumnAll(byte col, byte value)
{
	digitalWrite(load, LOW);    
	for (int i=0; i<num; i++) 
	{
		shiftOut(data, clock, MSBFIRST, col + 1);
		shiftOut(data, clock, MSBFIRST, value);
		buffer[col * i] = value;
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
}

void DWMaxMatrix::setDot(byte col, byte row, byte value)
{
    bitWrite(buffer[col], row, value);

	int n = col / 8;
	int c = col % 8;
	digitalWrite(load, LOW);    
	for (int i=0; i<num; i++) 
	{
		if (i == n)
		{
			shiftOut(data, clock, MSBFIRST, c + 1);
			shiftOut(data, clock, MSBFIRST, buffer[col]);
		}
		else
		{
			shiftOut(data, clock, MSBFIRST, 0);
			shiftOut(data, clock, MSBFIRST, 0);
		}
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
}

void DWMaxMatrix::writeSprite(int x, int y, const byte* sprite)
{
	int w = sprite[0];
	int h = sprite[1];
	
	if (h == 8 && y == 0)
		for (int i=0; i<w; i++)
		{
			int c = x + i;
			if (c>=0 && c<80)
				setColumn(c, sprite[i+2]);
		}
	else
		for (int i=0; i<w; i++)
			for (int j=0; j<h; j++)
			{
				int c = x + i;
				int r = y + j;
				if (c>=0 && c<80 && r>=0 && r<8)
					setDot(c, r, bitRead(sprite[i+2], j));
			}
}

void DWMaxMatrix::reload()
{
	for (int i=0; i<8; i++)
	{
		int col = i;
		digitalWrite(load, LOW);    
		for (int j=0; j<num; j++) 
		{
			shiftOut(data, clock, MSBFIRST, i + 1);
			shiftOut(data, clock, MSBFIRST, buffer[col]);
			col += 8;
		}
		digitalWrite(load, LOW);
		digitalWrite(load, HIGH);
	}
}

void DWMaxMatrix::shiftLeft(bool rotate, bool fill_zero)
{
	byte old = buffer[0];
	int i;
	for (i=0; i<80; i++)
		buffer[i] = buffer[i+1];
	if (rotate) buffer[num*8-1] = old;
	else if (fill_zero) buffer[num*8-1] = 0;
	
	reload();
}

void DWMaxMatrix::shiftRight(bool rotate, bool fill_zero)
{
	int last = num*8-1;
	byte old = buffer[last];
	int i;
	for (i=79; i>0; i--)
		buffer[i] = buffer[i-1];
	if (rotate) buffer[0] = old;
	else if (fill_zero) buffer[0] = 0;
	
	reload();
}

void DWMaxMatrix::shiftUp(bool rotate)
{
	for (int i=0; i<num*8; i++)
	{
		bool b = buffer[i] & 1;
		buffer[i] >>= 1;
		if (rotate) bitWrite(buffer[i], 7, b);
	}
	reload();
}

void DWMaxMatrix::shiftDown(bool rotate)
{
	for (int i=0; i<num*8; i++)
	{
		bool b = buffer[i] & 128;
		buffer[i] <<= 1;
		if (rotate) bitWrite(buffer[i], 0, b);
	}
	reload();
}





