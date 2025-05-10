#ifndef TFT_8050A_H
#define TFT_8050A_H



const struct {
  const String version  = "Firmware v0.1";
  const String hardware = "8050A";
  const String author   = "Trikalinos"; 
  const String github1  = "github.com/ttrikalin/";
  const String github2  = "fluke8050A-yada";
} firmware_information; 


#include "NotoSansBold36.h"
#include "NotoSansBold15.h"



#define AA_FONT_SMALL  NotoSansBold15
#define AA_FONT_MEDIUM NotoSansBold36
#define AA_FONT_LARGE  PTSansNarrow36

#include "00_symbol_splash.h"
#include "01_diode_27_19.h"
#include "02_battery_41_16.h"
#include "03_HV_42_36.h"
#include "04_large_digits_signs.h"
#include "05_small_digits_signs.h"
#include "06_symbol_unit.h"
#include "07_symbol_mode.h"

#include <TFT_eSPI.h> 
#include <SPI.h>

// // bit clear/set/invert/test
#define clear_bit(b,bit) (b &= ~_BV(bit))
#define set_bit(b,bit) (b |= _BV(bit))
#define invert_bit(b,bit) (b ^= _BV(bit))
#define test_bit(b,bit) ((b & _BV(bit)) != 0)




/**************** FLUKE 8050A config start *********************/
#define fluke8050a_WITH_BATTERY 1
#define fluke8050a_HARDWARE_IMPEDANCE_REFERENCE_STR " 600"
/**************** FLUKE 8050A config end ***********************/

/**************** ESP32-WROOM32 configuration start ************/
// 8050A range and function switches
// rng 
#define fluke8050a_RNG_C   19
#define fluke8050a_RNG_B   18
#define fluke8050a_RNG_A   5
#define fluke8050a_BT      17     // TX2

// func 
#define fluke8050a_FUNC_B  16     // RX2
#define fluke8050a_FUNC_D  4
#define fluke8050a_FUNC_A  2
#define fluke8050a_FUNC_C  15

// 8050A strobe lines
#define fluke8050a_ST0     13
#define fluke8050a_ST1     12
#define fluke8050a_ST2     14
#define fluke8050a_ST3     27
#define fluke8050a_ST4     26

// 8050A scancode lines
#define fluke8050a_HV      25
#define fluke8050a_DP      33
#define fluke8050a_Z       32
#define fluke8050a_Y       35
#define fluke8050a_X       34
#define fluke8050a_W       39      // VN
// #define BIT_1           fluke8050a_Z
// #define BIT_PLUS        fluke8050a_X
// #define BIT_MINUS       fluke8050a_W

// // These are defined in the User_Setup.h
// #define TFT_SCLK 21
// #define TFT_MOSI  3  // (RX0) Also called SDI 
// #define TFT_DC    1  // (TX0) Data Command or register select (RS) control pin
// #define TFT_RST  22  //       Reset pin (could connect to RST pin)
// #define TFT_CS   23  //       Chip select control pin

/* Function prototypes */
void IRAM_ATTR strobe0_ISR(void);
void IRAM_ATTR strobe1_ISR(void);
void IRAM_ATTR strobe2_ISR(void);
void IRAM_ATTR strobe3_ISR(void);
void IRAM_ATTR strobe4_ISR(void);
void ESP32_WROOM32_initialize(void); 
void display_initialize(void); 
/**************** ESP32-WROOM32 configuration end ************/

/***************** Function monitor start ***********************/
/* Function states */
typedef enum  {
    FUNCTION_MONITOR_STATE_INIT = 0, 
    FUNCTION_MONITOR_STATE_WAIT = 1, 
    FUNCTION_MONITOR_STATE_READ = 2
} function_monitor_states; 


/* Function data */
typedef enum {                      // 0bDCBA
    RELATIVE_RESISTANCE_0 =  0,     // 0b0000
    RELATIVE_RESISTANCE_1 =  1,     // 0b0001
    RELATIVE_DC_VOLTAGE   =  2,     // 0b0010 
    RELATIVE_AC_VOLTAGE   =  3,     // 0b0011
    RELATIVE_DC_CURRENT   =  4,     // 0b0100 
    RELATIVE_AC_CURRENT   =  5,     // 0b0101   
    GAIN_DB_0             =  6,     // 0b0110
    GAIN_DB_1             =  7,     // 0b0111
    ABSOLUTE_RESISTANCE_0 =  8,     // 0b1000
    ABSOLUTE_RESISTANCE_1 =  9,     // 0b1001
    ABSOLUTE_DC_VOLTAGE   = 10,     // 0b1010
    ABSOLUTE_AC_VOLTAGE   = 11,     // 0b1011
    ABSOLUTE_DC_CURRENT   = 12,     // 0b1100
    ABSOLUTE_AC_CURRENT   = 13,     // 0b1101
    GAIN_DB_2             = 14,     // 0b1110
    GAIN_DB_3             = 15      // 0b1111
} measurement_function; 

typedef struct {
    function_monitor_states state; 
    // unsigned char read_flag; 
    measurement_function active_function; 
} functionMonitorData; 

/* Function prototypes */ 
void function_monitor_initialize(void); 
void function_monitor_tasks(void); 
/***************** Function monitor end ***********************/

/***************** Range monitor start ************************/
/* Range states */
typedef enum  {
    RANGE_MONITOR_STATE_INIT = 0, 
    RANGE_MONITOR_STATE_WAIT = 1, 
    RANGE_MONITOR_STATE_READ = 2
} range_monitor_states; 

/* Range data */
typedef enum {              // 0b0CBA
    RANGE_20M     =  0,     // 0b0000
    RANGE_200nS   =  1,     // 0b0001
    RANGE_2mS     =  2,     // 0b0010 
    RANGE_POINT_2 =  3,     // 0b0011
    RANGE_200     =  4,     // 0b0100 
    RANGE_20      =  5,     // 0b0101   
    RANGE_2       =  6,     // 0b0110
    RANGE_2000    =  7      // 0b0111
} measurement_range; 

typedef struct {
    range_monitor_states state; 
    //unsigned char read_flag; 
    measurement_range active_range; 
} rangeMonitorData; 

/* Function prototypes */ 
void range_monitor_initialize(void); 
void range_monitor_tasks(void); 
/***************** Range monitor end **************************/

/***************** Digits monitor start ***********************/
/* Digits states */
typedef enum  {
    DIGITS_MONITOR_STATE_INIT  = 0, 
    DIGITS_MONITOR_STATE_WAIT  = 1, 
    DIGITS_MONITOR_STATE_READ  = 2
} digits_monitor_states; 

typedef enum {
    NO_STROBE = 0, 
    ST0_0     = 1, 
    ST0_1     = 2, 
    ST0_2     = 3, 
    ST1       = 4, 
    ST2       = 5, 
    ST3       = 6, 
    ST4       = 7
} strobe_number; 

/* Digits data */
typedef struct {
  digits_monitor_states state; 
  strobe_number read_strobe;
  unsigned char in_strobe_phase;
  unsigned char read_flag; 
  unsigned int st0_value0;
  unsigned int st0_value1;
  unsigned int st0_value2; 
  unsigned int st1_value;
  unsigned int st2_value; 
  unsigned int st3_value; 
  unsigned int st4_value; 
} digitsMonitorData; 

/* Function prototypes */ 
void digits_monitor_initialize(void); 
void digits_monitor_tasks(void); 
/***************** Digits monitor end *************************/

/***************** Contents monitor start *********************/
/* Contents states */
typedef enum  {
  CONTENTS_MONITOR_STATE_INIT  = 0, 
  CONTENTS_MONITOR_STATE_WAIT  = 1, 
  CONTENTS_MONITOR_STATE_INFER = 2
} contents_monitor_states; 

/* Contents data */
typedef enum  {
  NO_UNIT       =  0, 
  VOLT          =  1, 
  MILLI_VOLT    =  2, 
  MICRO_AMPERE  =  3, 
  MILLI_AMPERE  =  4, 
  OHM           =  5,
  KILO_OHM      =  6,
  MEGA_OHM      =  7,
  DECIBEL       =  8, 
  MILLI_SIEMENS =  9, 
  NANO_SIEMENS  = 10, 
  IMPENDANCE_Z  = 11
} units; 

typedef enum {
  NO_DIODE    = 0, 
  DIODE       = 1, 
  SMALL_DIODE = 2
} diode_styles; 

typedef enum {
  NO_QUANTITY = 0, 
  RESISTANCE  = 1, 
  CONDUCTANCE = 2, 
  CURRENT     = 3, 
  VOLTAGE     = 4,
  GAIN        = 5
} quantities;

typedef enum {
  DECIMAL_POINT_AT_ZERO  = 0, 
  DECIMAL_POINT_AT_ONE   = 1, 
  DECIMAL_POINT_AT_TWO   = 2, 
  DECIMAL_POINT_AT_THREE = 3, 
  NO_DECIMAL_POINT       = 4
} decimal_point_positions;

typedef enum {
  POSITIVE_SIGN = 0,
  NEGATIVE_SIGN = 1,
  NO_SIGN       = 2 
} signs;

typedef enum {
  NORMAL_BATTERY = 0, 
  LOW_BATTERY = 1, 
  NO_BATTERY = 2 
} battery_styles;

typedef struct { 
  contents_monitor_states state; 
  units unit; 
  quantities quantity; 
  diode_styles diode_style;
  decimal_point_positions decimal_point_position;
  signs sign; 
  battery_styles battery;
  unsigned char alternating_current;
  unsigned char high_voltage; 
  unsigned char relative_measurement;
} contentsMonitorData; 

void contents_monitor_initialize(void); 
void contents_monitor_tasks(void); 
void infer_alternating_current(void); 
void infer_unit(void); 
void infer_quantity(void); 
void infer_diode_style(void); 
void infer_high_voltage(void); 
void infer_low_battery(void); 
void infer_relative_measurement(void); 
void infer_sign(void); 
void infer_decimal_point_position(void); 
/***************** Contents monitor end ***********************/


#define TFT_WIDTH           220
#define TFT_HEIGHT          176
#define TFT_SCREEN_ROTATION   3

#define TFT_SPLASH_SCREEN_DURATION 2000


// TOP ZONE 
#define X_HIGH_VOLTAGE  0
#define Y_HIGH_VOLTAGE  0

#define X_DIODE  ((TFT_WIDTH-DIODE_WIDTH)>>1)
#define Y_DIODE  0

#define X_BATTERY (TFT_WIDTH-BATTERY_WIDTH)
#define Y_BATTERY 0

// SECOND ZONE 
#define X_UNIT    10
#define Y_UNIT    (BATTERY_HEIGHT+3)

// MAIN MEASUREMENT ZONE 

#define X_ZONE_ONE  0
#define Y_ZONE_ONE  44
#define ZONE_ONE_HEIGHT  180
#define ZONE_ONE_WIDTH  220


#define X_ACDC    (X_UNIT-8)
#define Y_ACDC    (Y_ZONE_ONE+OFFSET_MODE_LG)

#define X_Z       166
#define Y_Z       128

#define X_REL     0
#define Y_REL     160


#define W_IMG_MAIN (W_SIGN_LG+W_DP_LG+4*W_DIGIT_LG)
#define H_IMG_MAIN (H_SIGN_LG+H_DP_LG+4*H_DIGIT_LG)




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


/* Display states */
typedef enum  {
  DISPLAY_MONITOR_STATE_INIT                 = 0, 
  DISPLAY_MONITOR_STATE_WAIT                 = 1, 
  DISPLAY_MONITOR_STATE_SHOW_VALUE           = 2, 
  DISPLAY_MONITOR_STATE_SHOW_REFERENCE_VALUE = 3, 
  DISPLAY_MONITOR_STATE_SHOW_HIGH_VOLTAGE    = 4, 
  DISPLAY_MONITOR_STATE_SHOW_LOW_BATTERY     = 5,
  DISPLAY_MONITOR_STATE_SHOW_DIODE           = 6,
  DISPLAY_MONITOR_STATE_SHOW_IMPEDANCES      = 7
} display_monitor_states; 

typedef struct {
  unsigned int splash_background_color;
  unsigned int splash_text_color;
  unsigned int voltage_background_color;
  unsigned int voltage_text_color;
  unsigned int current_background_color;
  unsigned int current_text_color;
  unsigned int resistance_background_color;
  unsigned int resistance_text_color;
  unsigned int conductance_background_color;
  unsigned int conductance_text_color;
  unsigned int gain_background_color;
  unsigned int gain_text_color;
  unsigned int invalid_background_color;
  unsigned int invalid_text_color;
} color_themes;

typedef struct {
  display_monitor_states state; 

  color_themes high_voltage_theme;
  color_themes non_high_voltage_theme;
  unsigned int active_background_color; 
  unsigned int active_text_color; 

  String digits_str; 
  float relative_reference;
  String impedance_reference_str;
} displayMonitorData; 


typedef struct {
  unsigned char width; 
  unsigned char height;
  unsigned char y_offset;
  const uint8_t * symbol; 
} oneSymbol; 


typedef struct {
  unsigned char width; 
  unsigned char height;
  unsigned char y_offset;
  const uint8_t * * symbols; 
} arrayOfSymbols; 



void display_monitor_initialize(void);
void display_monitor_tasks(void);
void use_colors(unsigned int background_color, 
                unsigned int text_color);
void update_colors(void);
void show_splash_screen(void); 
void format_large_digits(void); 
//void format_main(void);

//void draw_large_sign(signs sign, unsigned char &x, unsigned char &y);
void draw_using_one_symbol(TFT_eSprite &sprite, oneSymbol &one_symbol, bool invert_colors, unsigned char &x, unsigned char &y);
void draw_using_array_of_symbols(TFT_eSprite &sprite, arrayOfSymbols &array_of_symbols, unsigned char d, bool invert_colors, unsigned char &x, unsigned char &y);
//void show_digits(void); 
void show_reference_value(void); 
//void show_unit(void); 
void show_high_voltage(void);
void show_battery(void); 
void show_diode(void); 
void show_impedances(void); 



/***************** Display monitor end ************************/

#endif // TFT_8050A_H
