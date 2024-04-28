#ifndef ESP32_WROOM_H
#define ESP32_WROOM_H


// 8050A range and function switches
// rng 
#define fluke8050a_RNG_C   19
#define fluke8050a_RNG_B   18
#define fluke8050a_RNG_A   5
#define fluke8050a_BT      17     // TX2

// func 
#define fluke8050a_FUNC_B     16     // RX2
#define fluke8050a_FUNC_D     4
#define fluke8050a_FUNC_A     2
#define fluke8050a_FUNC_C     15

// 8050A strobe lines
#define fluke8050a_ST0         13
#define fluke8050a_ST1         12
#define fluke8050a_ST2         14
#define fluke8050a_ST3         27
#define fluke8050a_ST4         26

// 8050A scancode lines
#define fluke8050a_HV          25
#define fluke8050a_DP          33
#define fluke8050a_Z           32
#define fluke8050a_Y           35
#define fluke8050a_X           34
#define fluke8050a_W           39      // VN
#define BIT_1           fluke8050a_Z
#define BIT_PLUS        fluke8050a_X
#define BIT_MINUS       fluke8050a_W

// // These are defined in the User_Setup.h
// #define TFT_SCLK 21
// #define TFT_MOSI  3  // (RX0) Also called SDI 
// #define TFT_DC    1  // (TX0) Data Command or register select (RS) control pin
// #define TFT_RST  22  //       Reset pin (could connect to RST pin)
// #define TFT_CS   23  //       Chip select control pin

#endif // ESP32_WROOM_H
