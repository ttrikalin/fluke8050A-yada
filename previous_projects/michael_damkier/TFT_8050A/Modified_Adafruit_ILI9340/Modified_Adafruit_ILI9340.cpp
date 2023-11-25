/*
Modified for TFT_8050A_01
*/

/***************************************************
  This is an Arduino Library for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "Modified_Adafruit_ILI9340.h"
#include <avr/pgmspace.h>
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <SPI.h>

#define SET_BIT(port, bitMask) *(port) |= (bitMask)
#define CLEAR_BIT(port, bitMask) *(port) &= ~(bitMask)

// Constructor when using hardware SPI.  Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
Modified_Adafruit_ILI9340::Modified_Adafruit_ILI9340(uint8_t dc, uint8_t rst) {
  _dc   = dc;
  _rst  = rst;
  _mosi  = _sclk = 0;
}

void Modified_Adafruit_ILI9340::spiwrite(uint8_t c) {

  //Serial.print("0x"); Serial.print(c, HEX); Serial.print(", ");

  SPDR = c;
  while(!(SPSR & _BV(SPIF)));
}


void Modified_Adafruit_ILI9340::writecommand(uint8_t c) {
  CLEAR_BIT(dcport, dcpinmask);
  //digitalWrite(_dc, LOW);
  CLEAR_BIT(clkport, clkpinmask);
  //digitalWrite(_sclk, LOW);

  spiwrite(c);

}


void Modified_Adafruit_ILI9340::writedata(uint8_t c) {
  SET_BIT(dcport,  dcpinmask);
  //digitalWrite(_dc, HIGH);
  CLEAR_BIT(clkport, clkpinmask);
  //digitalWrite(_sclk, LOW);
  
  spiwrite(c);

} 

void Modified_Adafruit_ILI9340::begin(void) {
  pinMode(_rst, OUTPUT);
  digitalWrite(_rst, LOW);

  pinMode(_dc, OUTPUT);
  dcport    = portOutputRegister(digitalPinToPort(_dc));
  dcpinmask = digitalPinToBitMask(_dc);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz (full! speed!)
  SPI.setBitOrder(MSBFIRST);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  // toggle RST low to reset

  digitalWrite(_rst, HIGH);
  delay(5);
  digitalWrite(_rst, LOW);
  delay(20);
  digitalWrite(_rst, HIGH);
  delay(150);

  writecommand(0xEF);
  writedata(0x03);
  writedata(0x80);
  writedata(0x02);

  writecommand(0xCF);  
  writedata(0x00); 
  writedata(0XC1); 
  writedata(0X30); 

  writecommand(0xED);  
  writedata(0x64); 
  writedata(0x03); 
  writedata(0X12); 
  writedata(0X81); 
 
  writecommand(0xE8);  
  writedata(0x85); 
  writedata(0x00); 
  writedata(0x78); 

  writecommand(0xCB);  
  writedata(0x39); 
  writedata(0x2C); 
  writedata(0x00); 
  writedata(0x34); 
  writedata(0x02); 
 
  writecommand(0xF7);  
  writedata(0x20); 

  writecommand(0xEA);  
  writedata(0x00); 
  writedata(0x00); 
 
  writecommand(ILI9340_PWCTR1);    //Power control 
  writedata(0x23);   //VRH[5:0] 
 
  writecommand(ILI9340_PWCTR2);    //Power control 
  writedata(0x10);   //SAP[2:0];BT[3:0] 
 
  writecommand(ILI9340_VMCTR1);    //VCM control 
  writedata(0x3e); //�Աȶȵ���
  writedata(0x28); 
  
  writecommand(ILI9340_VMCTR2);    //VCM control2 
  writedata(0x86);  //--
 
  writecommand(ILI9340_MADCTL);    // Memory Access Control 
  writedata(ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);

  writecommand(ILI9340_PIXFMT);    
  writedata(0x55); 
  
  writecommand(ILI9340_FRMCTR1);    
  writedata(0x00);  
  writedata(0x18); 
 
  writecommand(ILI9340_DFUNCTR);    // Display Function Control 
  writedata(0x08); 
  writedata(0x82);
  writedata(0x27);  
 
  writecommand(0xF2);    // 3Gamma Function Disable 
  writedata(0x00); 
 
  writecommand(ILI9340_GAMMASET);    //Gamma curve selected 
  writedata(0x01); 
 
  writecommand(ILI9340_GMCTRP1);    //Set Gamma 
  writedata(0x0F); 
  writedata(0x31); 
  writedata(0x2B); 
  writedata(0x0C); 
  writedata(0x0E); 
  writedata(0x08); 
  writedata(0x4E); 
  writedata(0xF1); 
  writedata(0x37); 
  writedata(0x07); 
  writedata(0x10); 
  writedata(0x03); 
  writedata(0x0E); 
  writedata(0x09); 
  writedata(0x00); 
  
  writecommand(ILI9340_GMCTRN1);    //Set Gamma 
  writedata(0x00); 
  writedata(0x0E); 
  writedata(0x14); 
  writedata(0x03); 
  writedata(0x11); 
  writedata(0x07); 
  writedata(0x31); 
  writedata(0xC1); 
  writedata(0x48); 
  writedata(0x08); 
  writedata(0x0F); 
  writedata(0x0C); 
  writedata(0x31); 
  writedata(0x36); 
  writedata(0x0F); 

  writecommand(ILI9340_SLPOUT);    //Exit Sleep 
  delay(120); 		
  writecommand(ILI9340_DISPON);    //Display on 
}

void Modified_Adafruit_ILI9340::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
 uint16_t y1) {

  writecommand(ILI9340_CASET); // Column addr set
  writedata(x0 >> 8);
  writedata(x0 & 0xFF);     // XSTART 
  writedata(x1 >> 8);
  writedata(x1 & 0xFF);     // XEND

  writecommand(ILI9340_PASET); // Row addr set
  writedata(y0>>8);
  writedata(y0);     // YSTART
  writedata(y1>>8);
  writedata(y1);     // YEND

  writecommand(ILI9340_RAMWR); // write to RAM
}

// This function displays a bitmap (byte array)
// at the given coordinates.
void Modified_Adafruit_ILI9340::bmpDraw( 
  const uint8_t * b,
  uint16_t x, uint16_t y, 
  uint16_t w, uint16_t h,
  uint16_t fg, uint16_t bg )
{
  // Set TFT address window to image bounds
  setAddrWindow( x, y, x + w - 1, y + h - 1 );
  
  SET_BIT(dcport, dcpinmask);
  //digitalWrite(_dc, HIGH);

  uint8_t fgHi = fg >> 8;
  uint8_t fgLo = fg;
  uint8_t bgHi = bg >> 8;
  uint8_t bgLo = bg;
  
  uint16_t row;
  uint16_t col;
  
  if ( b == 0 ) // no bitmap, just fill with bg color
  {
    for ( row = 0; row < h; row++ )
    {
      for ( col = 0; col < w; col++ )
      {
        spiwrite(bgHi);
        spiwrite(bgLo);
      }
    }
  }
  else
  {
    uint16_t pos = 0;
    uint8_t bits;
    for ( row = 0; row < h; row++ )
    {
      uint8_t mask = B10000000;
      for ( col = 0; col < w; col++ )
      {
        if ( mask == B10000000 )
        {
          bits = pgm_read_byte(b + pos++);
        }
        if ( bits & mask )
        {
          spiwrite(fgHi);
          spiwrite(fgLo);
        }
        else
        {
          spiwrite(bgHi);
          spiwrite(bgLo);
        }
        mask >>= 1;
        if ( mask == 0 )
        {
          mask = B10000000;
        }
      }
    }
  }
}

void Modified_Adafruit_ILI9340::fillScreen(uint16_t color) {
  bmpDraw(0, 0, 0, _width, _height, 0, color);
}

void Modified_Adafruit_ILI9340::setRotation(uint8_t m) {

  writecommand(ILI9340_MADCTL);
  m %= 4; // can't be higher than 3
  switch (m) {
   case 0:
     writedata(ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);
     _width  = ILI9340_TFTWIDTH;
     _height = ILI9340_TFTHEIGHT;
     break;
   case 1:
     writedata(ILI9340_MADCTL_MV | ILI9340_MADCTL_BGR);
     _width  = ILI9340_TFTHEIGHT;
     _height = ILI9340_TFTWIDTH;
     break;
  case 2:
    writedata(ILI9340_MADCTL_MY | ILI9340_MADCTL_BGR);
     _width  = ILI9340_TFTWIDTH;
     _height = ILI9340_TFTHEIGHT;
    break;
   case 3:
     writedata(ILI9340_MADCTL_MV | ILI9340_MADCTL_MY | ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);
     _width  = ILI9340_TFTHEIGHT;
     _height = ILI9340_TFTWIDTH;
     break;
  }
}
