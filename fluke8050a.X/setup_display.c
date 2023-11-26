#include "setup_ili9341.h"

void display_write_command (unsigned int data){
  DISPLAY_DC = 0; 
  DISPLAY_CS = 0;
  
  SSPBUF = (unsigned char) data;
  while(!SSPIF);
  SSPIF=0;
  
  DISPLAY_CS = 1;
}



// send 8 bits  of data 
void display_write_data (unsigned int data) {
  DISPLAY_DC = 1; 
  DISPLAY_CS = 0;
  
  SSPBUF = (unsigned char) data;
  while(!SSPIF);
  SSPIF = 0;

  DISPLAY_CS = 1;
}


// send 16 bits  of data 
void display_write_data16 (unsigned int data) {
  DISPLAY_DC = 1; 
  DISPLAY_CS = 0;
  
  SSPBUF= data>>8;
  while(!SSPIF);
  SSPIF=0;

  SSPBUF = (unsigned char) data;
  while(!SSPIF);
  SSPIF=0;  
  
  DISPLAY_CS = 1;
}

void delay_10ms_steps( unsigned int steps ){
  for(unsigned int i = 0; i < steps; i++){
    __delay_ms(10); 
  }
}

void display_clear_screen(void){
  unsigned long counter = 0;
  while(counter < DISPLAY_CAPACITY) { //clear screen
    counter++;
    display_write_data(0x00); 
    display_write_data(0x00);
  }
}

void display_setup(void){
  DISPLAY_RESET = 1;
  delay_10ms_steps(10);
  DISPLAY_RESET = 0;
  delay_10ms_steps(2);
  DISPLAY_RESET = 1;
  delay_10ms_steps(12);
  
  display_write_command(DISPLAY_OFF);  
  display_write_command(DISPLAY_OFF);  
  display_write_command(DISPLAY_OFF);  
  display_write_command(DISPLAY_OFF);  
  display_write_command(DISPLAY_OFF);  
  delay_10ms_steps(2);

  display_write_command(DISPLAY_POWER_CTRL_B);  
  display_write_data(0x00); 
  display_write_data(0x83);
  display_write_data(0x30);


  display_write_command(DISPLAY_POWER_ON_SEQUENCE_CTRL);
  display_write_data(0x64);
  display_write_data(0x03);
  display_write_data(0x12);
  display_write_data(0x81);

  display_write_command(DISPLAY_DRIVER_TIMING_CTRL_A); 
  display_write_data(0x85);
  display_write_data(0x01);
  display_write_data(0x79);

  display_write_command(DISPLAY_POWER_CTRL_A); 
  display_write_data(0x39);
  display_write_data(0X2C);
  display_write_data(0x00);
  display_write_data(0x34);
  display_write_data(0x02);

  display_write_command(DISPLAY_PUMP_RATIO_CTRL);
  display_write_data(0x20);

  display_write_command(DISPLAY_DRIVER_TIMING_CTRL_B); 
  display_write_data(0x00);
  display_write_data(0x00);

  display_write_command(DISPLAY_POWER_CTRL_1); 
  display_write_data(0x26);

  display_write_command(DISPLAY_POWER_CTRL_2);
  display_write_data(0x11);

  display_write_command(DISPLAY_VCOM_CTRL_1); 
  display_write_data(0x35);
  display_write_data(0x3E);

  display_write_command(DISPLAY_VCOM_CTRL_2); 
  display_write_data(0xbe);

  display_write_command(DISPLAY_FRAME_RATE_CTRL);
  display_write_data(0x00);
  display_write_data(0x1b);

  display_write_command(DISPLAY_FUNCTION_CTRL); 
  display_write_data(0x0A);
  display_write_data(0x82);
  display_write_data(0x27);
  display_write_data(0x00);

  display_write_command(DISPLAY_SET_ENTRY_MODE); 
  display_write_data(0x07);

  display_write_command(DISPLAY_SET_PIXEL_FORMAT);
  display_write_data(0x55);//16bit

  display_write_command(DISPLAY_SET_COLUMN_ADDRESS); 
  display_write_data(0x00);
  display_write_data(0x00);
  display_write_data(0x00);
  display_write_data(0xef);

  display_write_command(DISPLAY_SET_PAGE_ADDRESS); 
  display_write_data(0x00);
  display_write_data(0x00);
  display_write_data(0x01);
  display_write_data(0x3f);


  display_write_command(DISPLAY_SET_NORMAL_MODE);
  
  display_write_command(DISPLAY_MEMORY_ACCESS_CTRL);
  display_write_data(0b11011100);

  
  display_write_command(DISPLAY_SLEEP_OUT); 

  delay_10ms_steps(12);
  
  display_write_command(DISPLAY_ON); 

  delay_10ms_steps(2);

  display_write_command(DISPLAY_MEMORY_WRITE);
  display_clear_screen();  
}
