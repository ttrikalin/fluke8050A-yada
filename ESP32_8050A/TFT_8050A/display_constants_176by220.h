#ifndef _SRC_DISPLAY_CONSTANTS_H
#define _SRC_DISPLAY_CONSTANTS_H


/*
DISPLAY
*/

// colors
#define BG_COLOR        0x0200
#define FG_COLOR        0xFFFF
#define HV_COLOR        0xFBCF
#define BT_COLOR        0x0
// the base unit (as a color)
#define BASEUNIT_V      0x97EF    // Voltage, green
#define BASEUNIT_A      0xFC71    // Current, red
#define BASEUNIT_R      0xA50A    // Resistance, yellow
#define BASEUNIT_S      0x8410    // Conductance, grey
#define BASEUNIT_dB     0x8C71    // Decibel, blue
#define BASEUNIT_Z      0x3A99    // Impedance, blue
#define BASEUNIT_undef  0x0       // undefined, black

// display x,y coords
#define X_MAIN          8
#define Y_MAIN          30

#define X_UNIT          256
#define Y_UNIT          Y_MAIN+OFFSET_UNIT_LG

#define X_MODE          X_UNIT-8
#define Y_MODE          Y_MAIN+OFFSET_MODE_LG

#define X_Z             166
#define Y_Z             128

#define X_REL           98
#define Y_REL           176

#define X_LOBATT        X_MAIN
#define Y_LOBATT        Y_REL+16

// space between REL and digits
#define W_SPACE         12

// main digits underline
#define X_UNDERLINE     X_MAIN+W_SIGN_LG
#define Y_UNDERLINE     Y_MAIN+H_DIGIT_LG+8
#define W_UNDERLINE     5*W_DIGIT_LG+W_DP_LG
#define H_UNDERLINE     2

#endif // _SRC_DISPLAY_CONSTANTS_H

