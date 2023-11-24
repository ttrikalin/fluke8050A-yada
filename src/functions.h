
unsigned long bcd (unsigned int val); 
  
void ili9341_write_command (unsigned int data);
void ili9341_write_data (unsigned int data);

// send 16 bits  of data 
void ili9341_write_data16 (unsigned int data);

void print_number(unsigned int digit); 
void print_blank_columns(unsigned int blank_columns);
void print_decimal_point();
void print_small_characters(unsigned int digit);
void print_small_blank_columns(unsigned int blank_columns); 
void print_range_number(unsigned int digit);

