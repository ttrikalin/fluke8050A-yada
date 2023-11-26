# 1 "functions.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/Applications/microchip/mplabx/v6.15/packs/Microchip/PIC18Fxxxx_DFP/1.4.151/xc8/pic/include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "functions.c" 2
# 1 "./functions.h" 1



unsigned long bcd (unsigned int val);

void ili9341_write_command (unsigned int data);
void ili9341_write_data (unsigned int data);


void ili9341_write_data16 (unsigned int data);

void print_number(unsigned int digit);
void print_blank_columns(unsigned int blank_columns);
void print_decimal_point();
void print_small_characters(unsigned int digit);
void print_small_blank_columns(unsigned int blank_columns);
void print_range_number(unsigned int digit);
# 1 "functions.c" 2

