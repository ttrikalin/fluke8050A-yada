#include "setup_pic18f4420.h"

void mc_pin_setup(void){ 
  // A/D converter is off  
  ADCON0 = 0x00; 
    
  // All inputs digital (may need an analog input for tone)
  ADCON1 = 0b00001111; 
  ADCON2 = 0x00; 
  
  OSCCON  = 0b01110000; //8MHz
  OSCTUNE = 0b01000000; //use PLL x4  -> 32MHz 
  
  
  TRISA = 0xff;   /*all bits input */
  TRISB = 0xff;   /*all bits input */
  TRISC = 0b00010111; 
  TRISD = 0b11111011;
  TRISE = 0xff;   /*all bits input */
  
  DISPLAY_CS = 1;

  // Synchronous serial port that communicates with the display driver 
  SSPSTAT  = 0;  // status register 
  SSPCON1  = 0b00110000; // SPI host mode -- FOSC/4; idle state clock HIGH; enable serial port with SCK, SDOO, SDI and bar_SS as serial port pins  
} 