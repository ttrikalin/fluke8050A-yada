#ifndef TFT_8050A_H
#define TFT_8050A_H

#define IS_ODD(x) ((x) & 0x01)

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

#include "00_symbol_splash.h"
#include "01_diode_27_19.h"
#include "02_battery_41_16.h"
#include "03_HV_42_36.h"
#include "04_large_digits_signs_220x176.h"
#include "05_small_digits_signs.h"
#include "06_symbol_unit.h"
#include "07_symbol_mode.h"
#include "10_TFT_220x176.h"

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
  NO_VOLTAGE_LEVEL = 0,
  LOW_VOLTAGE = 1, 
  HIGH_VOLTAGE = 2
} voltage_levels; 

typedef enum {
  NO_ACDC = 0,
  DC = 1,
  AC = 2 
} acdc_modes; 


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

typedef enum {
  ABSOLUTE_MEASUREMENT = 0, 
  RELATIVE_MEASUREMENT = 1
} relative_measurement_styles; 

typedef struct { 
  contents_monitor_states state; 
  units unit; 
  quantities quantity; 
  diode_styles diode_style;
  decimal_point_positions decimal_point_position;
  signs sign; 
  battery_styles battery;
  acdc_modes acdc_mode;
  voltage_levels voltage_level;
  relative_measurement_styles relative_measurement;
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






/* Display states */
typedef enum  {
  DISPLAY_MONITOR_STATE_INIT                    = 0, 
  DISPLAY_MONITOR_STATE_WAIT                    = 1, 
  DISPLAY_MONITOR_STATE_SHOW_ZONE_0             = 2, 
  DISPLAY_MONITOR_STATE_SHOW_MEASUREMENT_ZONE_1 = 3, 
  DISPLAY_MONITOR_STATE_SHOW_ZONE_2             = 4, 
  DISPLAY_MONITOR_STATE_SHOW_ZONE_3             = 5, 
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
void draw_splash_screen(void); 
void format_zone_0(void);
void format_zone_1(void);
void format_zone_2(void);
void format_zone_3(void);

void draw_using_one_symbol(TFT_eSprite &sprite, oneSymbol &one_symbol, bool invert_colors, unsigned char &x, unsigned char &y);
void draw_using_array_of_symbols(TFT_eSprite &sprite, arrayOfSymbols &array_of_symbols, unsigned char d, bool invert_colors, unsigned char &x, unsigned char &y);
void draw_zone_0(void);
void draw_zone_1(void);
void draw_zone_2(void);
void draw_zone_3(void);

void draw_battery(void);
void draw_high_voltage(void);
void draw_diode(void);


/***************** Display monitor end ************************/

#endif // TFT_8050A_H
