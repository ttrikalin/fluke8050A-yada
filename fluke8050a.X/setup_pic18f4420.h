#ifndef _SRC_PIC18F4420_H_
#define _SRC_PIC18F4420_H_

#include <htc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000
#endif


#define DISPLAY_CS        PORTCbits.RC7
#define display_reset     PORTCbits.RC6
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


// VGA color palette
#define VGA_BLACK       0x0000
#define VGA_WHITE       0xFFFF
#define VGA_RED         0xF800
#define VGA_GREEN       0x0400
#define VGA_BLUE        0x001F
#define VGA_SILVER      0xC618
#define VGA_GRAY        0x8410
#define VGA_MAROON      0x8000
#define VGA_YELLOW      0xFFE0
#define VGA_OLIVE       0x8400
#define VGA_LIME        0x07E0
#define VGA_AQUA        0x07FF
#define VGA_TEAL        0x0410
#define VGA_NAVY        0x0010
#define VGA_FUCHSIA     0xF81F
#define VGA_PURPLE      0x8010
#define VGA_TRANSPARENT 0xFFFFFFFF


#define foreground_colour VGA_YELLOW
#define background_colour VGA_BLACK
#define ac_colour         VGA_WHITE
#define volts_colour      VGA_LIME
#define amps_colour       VGA_RED
#define ohms_colour       VGA_YELLOW
#define siemens_colour    VGA_WHITE
#define db_colour         VGA_AQUA
#define range_colour      VGA_WHITE




void mc_pin_setup(void); 

#endif
