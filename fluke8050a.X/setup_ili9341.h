#ifndef _SRC_ILI9341_H_
#define _SRC_ILI9341_H_

#include "fonts/fonts.h"
#include "setup_pic18f4420.h"
#include "config_color_scheme.h"

// Display is portrait mode
// Horizontal is 240 (short)
// Vertical is 320 (long)

#ifndef DISPLAY_HEIGHT
#define DISPLAY_HEIGHT 240
#endif

#ifndef DISPLAY_WIDTH
#define DISPLAY_WIDTH 320
#endif

#ifndef DISPLAY_CAPACITY
#define DISPLAY_CAPACITY 76800 // 240*320
#endif


#define DISPLAY_OFF 0x28 
#define DISPLAY_ON 0x29 

#define DISPLAY_SLEEP_OUT 0x11
#define DISPLAY_POWER_CTRL_A 0xCB
#define DISPLAY_POWER_CTRL_B 0xCF
#define DISPLAY_POWER_CTRL_1 0xC0
#define DISPLAY_POWER_CTRL_2 0xC1

#define DISPLAY_VCOM_CTRL_1 0xC5
#define DISPLAY_VCOM_CTRL_2 0xC7

#define DISPLAY_FRAME_RATE_CTRL 0xB1
#define DISPLAY_FUNCTION_CTRL 0xB6
#define DISPLAY_POWER_ON_SEQUENCE_CTRL 0xED
#define DISPLAY_DRIVER_TIMING_CTRL_A 0xE8
#define DISPLAY_DRIVER_TIMING_CTRL_B 0xEA
#define DISPLAY_PUMP_RATIO_CTRL 0xF7

#define DISPLAY_MEMORY_ACCESS_CTRL 0x36

#define DISPLAY_SET_ENTRY_MODE 0xB7
#define DISPLAY_SET_PIXEL_FORMAT 0x3A

#define DISPLAY_SET_COLUMN_ADDRESS 0x2a
#define DISPLAY_SET_PAGE_ADDRESS 0x2b
#define DISPLAY_MEMORY_WRITE 0x2c


#define DISPLAY_SET_NORMAL_MODE 0x13



unsigned int foreground_color, background_color;

void display_write_command (unsigned int data);
void display_write_data (unsigned int data);
void display_write_data16 (unsigned int data);
void display_clear_screen(void);
void print_character(unsigned int digit, enum u_fontsize fontsize);
void print_blank_columns(unsigned int blank_columns, enum u_fontsize fontsize);
void print_decimal_point(void); 
void display_setup(void); 
void delay_10ms_steps(unsigned int steps); 




#endif	

