#ifndef TFT_8050A_H
#define TFT_8050A_H


#include <TFT_eSPI.h> 
#include <SPI.h>
#include "ESP32_WROOM32_config_pins.h"
#include "fluke_8050a_function.h"
#include "fluke_8050a_values.h"
#include "fluke_8050a_range.h"
//#include "NotoSansBold15.h"
//#include "NotoSansBold36.h"

//#include "display_constants_176by220.h"


// bit clear/set/invert/test
#define clear_bit(b,bit) (b &= ~_BV(bit))
#define set_bit(b,bit) (b |= _BV(bit))
#define invert_bit(b,bit) (b ^= _BV(bit))
#define test_bit(b,bit) ((b & _BV(bit)) != 0)


#define NUM_DIGITS 5

// strobe lines
#define NUM_STROBES 5
#define ST0 0
#define ST1 1
#define ST2 2
#define ST3 3
#define ST4 4


//#define AA_FONT_SMALL NotoSansBold15
//#define AA_FONT_LARGE NotoSansBold36


void IRAM_ATTR strobe0_ISR(void);
void IRAM_ATTR strobe1_ISR(void);
void IRAM_ATTR strobe2_ISR(void);
void IRAM_ATTR strobe3_ISR(void);
void IRAM_ATTR strobe4_ISR(void);
void initialize_ESP32_WROOM32(void); 
void initialize_display(void); 







#endif // TFT_8050A_H
