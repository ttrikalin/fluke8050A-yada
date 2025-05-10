#ifndef TFT_220x176_H
#define TFT_220x176_H



#define TFT_WIDTH           220
#define TFT_HEIGHT          176
#define TFT_SCREEN_ROTATION   3

#define TFT_SPLASH_SCREEN_DURATION 2000

#define BETWEEN_ZONES_BUFFER_HEIGHT 6
#define WITHIN_ZONE_BUFFER_HEIGHT   3

#define X_ZONE_0  0
#define Y_ZONE_0  0
#define ZONE_0_WIDTH TFT_WIDTH
#define ZONE_0_HEIGHT HV_HEIGHT

#define X_ZONE_1  0
#define Y_ZONE_1  (Y_ZONE_0+ZONE_0_HEIGHT+BETWEEN_ZONES_BUFFER_HEIGHT)
#define ZONE_1_WIDTH  TFT_WIDTH
#define ZONE_1_HEIGHT H_DIGIT_LG

#define X_ZONE_2 0
#define Y_ZONE_2  (Y_ZONE_1+ZONE_1_HEIGHT+BETWEEN_ZONES_BUFFER_HEIGHT)
#define ZONE_2_WIDTH  TFT_WIDTH
#define ZONE_2_HEIGHT H_DIGIT_SM

#define X_ZONE_3 0
#define Y_ZONE_3  (Y_ZONE_2+ZONE_2_HEIGHT+BETWEEN_ZONES_BUFFER_HEIGHT)
#define ZONE_3_WIDTH  TFT_WIDTH
#define ZONE_3_HEIGHT H_DIGIT_SM

/** RELATIVE COORDINATES IN ZONE_0 **/
#define REL_IN_ZONE_X_HIGH_VOLTAGE  0
#define REL_IN_ZONE_Y_HIGH_VOLTAGE  0

#define REL_IN_ZONE_X_DIODE  ((TFT_WIDTH-DIODE_WIDTH)>>1)
#define REL_IN_ZONE_Y_DIODE  0

#define REL_IN_ZONE_X_BATTERY (TFT_WIDTH-BATTERY_WIDTH)
#define REL_IN_ZONE_Y_BATTERY 0


/** RELATIVE COORDINATES IN ZONE_1 **/

#define REL_IN_ZONE_X_MODE    0
#define REL_IN_ZONE_Y_MODE    0

#define REL_IN_ZONE_X_UNITS   0
#define REL_IN_ZONE_Y_UNITS   (WITHIN_ZONE_BUFFER_HEIGHT+H_MODE_LG)



// INCLUDED IN tft_espi
// #define TFT_BLACK       0x0000      /*   0,   0,   0 */
// #define TFT_NAVY        0x000F      /*   0,   0, 128 */
// #define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
// #define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
// #define TFT_MAROON      0x7800      /* 128,   0,   0 */
// #define TFT_PURPLE      0x780F      /* 128,   0, 128 */
// #define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
// #define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
// #define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
// #define TFT_BLUE        0x001F      /*   0,   0, 255 */
// #define TFT_GREEN       0x07E0      /*   0, 255,   0 */
// #define TFT_CYAN        0x07FF      /*   0, 255, 255 */
// #define TFT_RED         0xF800      /* 255,   0,   0 */
// #define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
// #define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
// #define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
// #define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
// #define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
// #define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
// #define TFT_BROWN       0x9A60      /* 150,  75,   0 */
// #define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
// #define TFT_SILVER      0xC618      /* 192, 192, 192 */
// #define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
// #define TFT_VIOLET      0x915C      /* 180,  46, 226 */
// #define TFT_TRANSPARENT 0x0120 // This is actually a dark green



#endif // TFT_220x176_H
