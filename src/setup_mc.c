#include "setup.h"

void mc_pin_setup(void){
  //no analog
  ANSELA  = 0;
  ANSELB  = 0;
  ANSELC  = 0;
  ANSELD  = 0;
  ANSELE  = 0;

  
  OSCCON  = 0b01110000; //16MHz
  OSCTUNE = 0b01000000; //use pll x4  ->64MHz woohoo let's fly
  
  
  TRISA = 0xff;   /*all bits input */
  TRISB = 0xff;   /*all bits input */
  TRISC = 0b00010111; 
  TRISD = 0b11111011;
  TRISE = 0xff;   /*all bits input */
  
  ili9341_cs = 1;

  SSP1STAT  = 0;
  SSP1CON1  = 0b00110000;
  
  number_display_foreground = foreground_colour;
  number_display_background = background_colour;  
}
  