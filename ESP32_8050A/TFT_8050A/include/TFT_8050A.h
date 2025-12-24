#ifndef TFT_8050A_H
#define TFT_8050A_H


#include <Arduino.h>
#include <esp_attr.h>
#include <String.h>

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
#include "11_TFT_320x240.h"
#include <TFT_eSPI.h> 
#include <SPI.h>

// // bit clear/set/invert/test
#define clear_bit(b,bit) (b &= ~_BV(bit))
#define set_bit(b,bit) (b |= _BV(bit))
#define invert_bit(b,bit) (b ^= _BV(bit))
#define test_bit(b,bit) ((b & _BV(bit)) != 0)




/**************** FLUKE 8050A config start *********************/
#define fluke8050a_WITH_BATTERY 
#define fluke8050a_HARDWARE_IMPEDANCE_REFERENCE_STR " 600"

//#define DEBUG_ENABLE
/**************** FLUKE 8050A config end ***********************/

/**************** ESP32-WROOM32 configuration start ************/
// 8050A range and function switches
// rng 
#define fluke8050a_RNG_C   36
#define fluke8050a_RNG_B   39
#define fluke8050a_RNG_A   34
#define fluke8050a_BT       2     

// func 
#define fluke8050a_FUNC_C  13
#define fluke8050a_FUNC_A  12
#define fluke8050a_FUNC_D  14
#define fluke8050a_FUNC_B  35     

// 8050A strobe lines
#define fluke8050a_ST0     27
#define fluke8050a_ST1     26
#define fluke8050a_ST2     25
#define fluke8050a_ST3     33
#define fluke8050a_ST4     32

// 8050A scancode lines
#define fluke8050a_HV      16
#define fluke8050a_DP       4
#define fluke8050a_Z       19
#define fluke8050a_Y       18
#define fluke8050a_X        5
#define fluke8050a_W       17      


#ifdef DEBUG_ENABLE
  #define DEBUG_LED         2
#endif
/** These are defined in the User_Setup.h
// #define TFT_SCLK 21
// #define TFT_MOSI  3  // (RX0) Also called SDI 
// #define TFT_DC    1  // (TX0) Data Command or register select (RS) control pin
// #define TFT_RST  22  //       Reset pin (could connect to RST pin)
// #define TFT_CS   23  //       Chip select control pin
***************************************************************/

/* Function prototypes */
void strobe0_ISR(void);
void strobe1_ISR(void);
void strobe2_ISR(void);
void strobe3_ISR(void);
void strobe4_ISR(void);
void ESP32_WROOM32_initialize(void); 
/**************** ESP32-WROOM32 configuration end ************/






/***************** Input monitor start ***********************/
// These tasks change rarely -- at a cadence slower than the measurement cadence. 
// e.g., if the user presses a switch or if the battery changes from low to high. 
// They should be checked and updated at a slow cadence.  
// The user inputs are manually set by the user -- this means that you need not check it all the time. 
// that you need not check it all the time -- maybe once per 500ms
// -- measurement functions (resistance, voltage, current, gain) 
// -- ranges and units (20M, 200nS, 2mS, Point 2, 200, 20, 2, 2000) 
// -- diode, battery 


//#define UPDATE_INPUTS_EVERY_N_LOOPS 8

/* slow tasks states */
typedef enum  {
    INPUTS_MONITOR_STATE_INIT = 0, 
    INPUTS_MONITOR_STATE_WAIT = 1, 
    INPUTS_MONITOR_STATE_READ = 2
} inputs_monitor_states; 


enum struct Function{                      
  INVALID    =  0,     
  VOLTAGE    =  1,     
  CURRENT    =  2,     
  RESISTANCE =  3,     
  GAIN       =  4     
};


/* Range data */
enum struct Range {              
  INVALID = 0,
  R_0P2   = 1,     
  R_2     = 2, 
  R_20    = 3,     
  R_200   = 4,     
  R_2000  = 5,
  R_20M   = 6,       
  R_200nS = 7,     
  R_2mS   = 8
}; 


enum struct BaseUnit {
  INVALID      = 0,
  VOLT         = 1,
  AMPERE       = 2,
  OHM          = 3,
  SIEMENS      = 4,
  DECIBEL      = 5, 
  IMPEDANCE_Z = 6
}; 

/* Units data */
enum struct Unit {
  NO_UNIT       = -1, 
  VOLT          =  0, 
  MILLI_VOLT    =  1, 
  MICRO_AMPERE  =  2, 
  MILLI_AMPERE  =  3, 
  OHM           =  4,
  KILO_OHM      =  5,
  MEGA_OHM      =  6,
  DECIBEL       =  7, 
  MILLI_SIEMENS =  8, 
  NANO_SIEMENS  =  9, 
  IMPEDANCE_Z   = 10
}; 


enum struct Mode {
  NO_ACDC = -1,
  DC = 0,
  AC = 1 
}; 

  
enum struct DiodeStyle {
  NO_DIODE    = -1, 
  DIODE       = 0, 
  SMALL_DIODE = 1
};

// enum struct DecimalPointPosition {
//   DECIMAL_POINT_AT_ZERO  = 0, 
//   DECIMAL_POINT_AT_ONE   = 1, 
//   DECIMAL_POINT_AT_TWO   = 2, 
//   DECIMAL_POINT_AT_THREE = 3, 
//   NO_DECIMAL_POINT       = 4
// };

enum struct Sign {
  NO_SIGN       = -1, 
  POSITIVE_SIGN = 0,
  NEGATIVE_SIGN = 1
};  

enum struct BatteryStyle {
  NO_BATTERY     = -1, 
  NORMAL_BATTERY = 0, 
  LOW_BATTERY    = 1
};

enum struct RelativeMeasurementStyle{
  RELATIVE_MEASUREMENT = 0,
  ABSOLUTE_MEASUREMENT = 1 
}; 

struct RawInputs {
  unsigned int range_bits; 
  unsigned int function_bits;
  bool AC_flag; 
  bool absolute_flag;
  bool battery_low_flag;
  bool new_values_flag;
}; 

struct InputsMonitorData {
  inputs_monitor_states state; 
  Function function; 
  Range range; 
  BaseUnit base_unit;
  Unit unit; 
  DiodeStyle diode_style;
  //DecimalPointPosition decimal_point_position;
  BatteryStyle battery;
  Mode acdc_mode;
  RelativeMeasurementStyle relative_measurement;
  bool new_values_flag; 
  bool valid_input_combination;
};

/* Function prototypes */ 
void inputs_monitor_initialize(void); 
void inputs_monitor_tasks(void); 
void read_raw_inputs(RawInputs &raw_inputs);
void decode_raw_inputs(const RawInputs &raw_inputs, InputsMonitorData &inputs_monitor_data);
Range infer_range(unsigned int range_bits); 
Function infer_function(unsigned int function_bits); 
bool is_valid_input_combination(Function function, Range range); 
//DecimalPointPosition infer_decimal_point_position(const Function function, const Range range);
DiodeStyle infer_diode_style(const Function function, const Range range);

/***************** Inputs monitor end ***********************/



/***************** Fast tasks monitor start ***********************/
// The fast tasks monitor state machine reads the strobe lines 
// and updates the fast tasks monitor data structure. 
// These change fast -- so you need to check it all the time, once per loop iteration. 
// also, it reads the voltage level, and the sign bit for the digits.


// /* Digits states */
// typedef enum  {
//   FAST_TASKS_MONITOR_STATE_INIT  = 0, 
//   FAST_TASKS_MONITOR_STATE_WAIT  = 1, 
//   FAST_TASKS_MONITOR_STATE_READ  = 2
// } fast_tasks_monitor_states; 

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


typedef enum {
NO_VOLTAGE_LEVEL = 0,
LOW_VOLTAGE = 1, 
HIGH_VOLTAGE = 2
} voltage_levels; 

/* Digits data */
struct FastTasksMonitorData {
  //fast_tasks_monitor_states state; 
  volatile strobe_number isr_active_strobe;  // Shared with ISR - must be volatile
  volatile unsigned int st0_value0;          // Shared with ISR - must be volatile
  volatile unsigned int st0_value1;          // Shared with ISR - must be volatile
  volatile unsigned int st0_value2;          // Shared with ISR - must be volatile
  volatile unsigned int st1_value;           // Shared with ISR - must be volatile
  volatile unsigned int st2_value;           // Shared with ISR - must be volatile
  volatile unsigned int st3_value;           // Shared with ISR - must be volatile
  volatile unsigned int st4_value;           // Shared with ISR - must be volatile
  volatile voltage_levels voltage_level; 
  volatile Sign sign; 
  volatile bool DP_flag0_0;
  volatile bool DP_flag0_1;
  volatile bool DP_flag0_2;
  volatile bool DP_flag1;
  volatile bool DP_flag2;
  volatile bool DP_flag3;
  volatile bool DP_flag4;
}; 

/* Function prototypes */ 
voltage_levels read_high_voltage(void); 
void infer_sign(void); 
void fast_tasks_monitor_initialize(void); 
//void fast_tasks_monitor_tasks(void);
void read_strobe(void); 
/***************** Fast tasks monitor end *************************/




/* Display states */
typedef enum  {
  DISPLAY_MONITOR_STATE_INIT                       = 0, 
  DISPLAY_MONITOR_STATE_WAIT                       = 1, 
  DISPLAY_MONITOR_STATE_UPDATE_BACKGROUND_STATUS   = 2, 
  DISPLAY_MONITOR_STATE_UPDATE_MEASUREMENT         = 3, 
  DISPLAY_MONITOR_STATE_INVALID_INPUTS             = 4, 
  DISPLAY_MONITOR_STATE_DEBUG_SCREEN               = 5
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
  unsigned int negative_meter_color;
} color_themes;

typedef struct {
  display_monitor_states state; 

  color_themes high_voltage_theme;
  color_themes non_high_voltage_theme;
  unsigned int active_background_color; 
  unsigned int active_text_color; 
  unsigned int active_negative_meter_color; 

  //String digits_str; 
  float relative_reference;
  //String impedance_reference_str;

} displayMonitorData; 


typedef struct {
  unsigned int width; 
  unsigned int height;
  unsigned int y_offset;
  const uint8_t * symbol; 
} oneSymbol; 


typedef struct {
  unsigned int width; 
  unsigned int height;
  unsigned int y_offset;
  const uint8_t * * symbols; 
} arrayOfSymbols; 

typedef struct {
  unsigned int x; 
  unsigned int y; 
} point; 

void display_monitor_initialize(void);
void display_monitor_tasks(void);
void use_colors(unsigned int background_color, 
                unsigned int text_color,
                unsigned int negative_meter_color);
void update_colors(void);
void draw_splash_screen(void); 


point draw_symbol_to_sprite(TFT_eSprite &sprite, oneSymbol &symbol, bool invert_colors, point p);
point draw_symbol_array_element_to_sprite(TFT_eSprite &sprite, arrayOfSymbols &array_of_symbols, unsigned int d, bool invert_colors, point p);
point draw_symbol_to_tft(TFT_eSPI &tft, oneSymbol &symbol, bool invert_colors, point p);
point draw_symbol_array_element_to_tft(TFT_eSPI &tft, arrayOfSymbols &array_of_symbols, unsigned int d, bool invert_colors, point p);



void draw_background_status_screen(void);
void draw_invalid_inputs_screen(void);
void draw_debug_screen(void);
void draw_measurement(void);
void draw_analog_meter(bool is_signed);


/***************** Display monitor end ************************/

#endif // TFT_8050A_H
