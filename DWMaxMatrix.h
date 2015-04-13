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

#ifndef _DWMaxMatrix_H_
#define _DWMaxMatrix_H_

#include "Arduino.h"

#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08
#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0a
#define max7219_reg_scanLimit   0x0b
#define max7219_reg_shutdown    0x0c
#define max7219_reg_displayTest 0x0f

class DWMaxMatrix
{
  private:
    byte data;
    byte load;
    byte clock;
    byte num;
    byte * buffer;

    void reload();

  public:
    DWMaxMatrix(byte load, byte clock, byte data,  byte num, byte * colBuffer);

    void init();
    void clear();
    void setCommand(byte command, byte value);
    void setIntensity(byte intensity);
    void setColumn(byte col, byte value);
    void setColumnAll(byte col, byte value);
    void setDot(byte col, byte row, byte value);
    void writeSprite(int x, int y, const byte* sprite);

    void shiftLeft(bool rotate = false, bool fill_zero = true);
    void shiftRight(bool rotate = false, bool fill_zero = true);
    void shiftUp(bool rotate = false);
    void shiftDown(bool rotate = false);
};

#endif



