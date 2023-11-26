#ifndef _SRC_PIC18F4420_H_
#define _SRC_PIC18F4420_H_

#include <htc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000
#endif


#define DISPLAY_CS        PORTCbits.RC7
#define DISPLAY_RESET     PORTCbits.RC6
#define DISPLAY_DC        PORTDbits.RD2
//    display_soi       pin 24 SDO1
//    display_clk       pin 18 SCK1

#define fluke8050a_ST0      PORTCbits.RC1
#define fluke8050a_ST1      PORTCbits.RC0
#define fluke8050a_ST2      PORTAbits.RA6
#define fluke8050a_ST3      PORTAbits.RA7
#define fluke8050a_ST4      PORTEbits.RE2
#define fluke8050a_HV       PORTCbits.RC4
#define fluke8050a_FUNC_A   PORTCbits.RC2
#define fluke8050a_FUNC_B   PORTAbits.RA4
#define fluke8050a_FUNC_C   PORTDbits.RD0
//#define fluke8050a_FUNC_D   not used


#define fluke8050a_W      PORTBbits.RB1
#define fluke8050a_X      PORTBbits.RB2
#define fluke8050a_Y      PORTBbits.RB3
#define fluke8050a_Z      PORTBbits.RB4
#define fluke8050a_DP     PORTDbits.RD3

#define fluke8050a_RNG_A    PORTAbits.RA1
#define fluke8050a_RNG_B    PORTAbits.RA2
#define fluke8050a_RNG_C    PORTAbits.RA3


void mc_pin_setup(void); 

#endif
