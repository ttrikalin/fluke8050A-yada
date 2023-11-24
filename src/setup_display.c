#include "setup.h"

void ili9341_setup(void){
    ili9341_reset = 1;
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  ili9341_reset = 0;
  __delay_ms(10);
  __delay_ms(10);
  ili9341_reset = 1;
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  
  ili9341_write_command(0x28);  //display off
  ili9341_write_command(0x28);  //display off
  ili9341_write_command(0x28);  //display off
  ili9341_write_command(0x28);  //display off
  
  ili9341_write_command(0x28);  //display off
  __delay_ms(10);
  __delay_ms(10);

  ili9341_write_command(0xCF);  //POWER_CTRLB
  ili9341_write_data(0x00);
  ili9341_write_data(0x83);
  ili9341_write_data(0x30);


  ili9341_write_command(0xED);  //LCD_CMD_POWERON_SEQ_CTRL
  ili9341_write_data(0x64);
  ili9341_write_data(0x03);
  ili9341_write_data(0x12);
  ili9341_write_data(0x81);

  ili9341_write_command(0xE8);  //LCD_CMD_DRV_TIMING_CTRLA
  ili9341_write_data(0x85);
  ili9341_write_data(0x01);
  ili9341_write_data(0x79);

  ili9341_write_command(0xCB);  //LCD_CMD_POWER_CTRLA
  ili9341_write_data(0x39);
  ili9341_write_data(0X2C);
  ili9341_write_data(0x00);
  ili9341_write_data(0x34);
  ili9341_write_data(0x02);

  ili9341_write_command(0xF7);  //LCD_CMD_PUMP_RATIO_CTRL
  ili9341_write_data(0x20);

  ili9341_write_command(0xEA);  //LCD_CMD_DRV_TIMING_CTRLB
  ili9341_write_data(0x00);
  ili9341_write_data(0x00);

  ili9341_write_command(0xC0);  //LCD_CMD_POWER_CTRL1
  ili9341_write_data(0x26);

  ili9341_write_command(0xC1);  //LCD_CMD_POWER_CTRL2
  ili9341_write_data(0x11);

  ili9341_write_command(0xC5);  //LCD_CMD_VCOM_CTRL1
  ili9341_write_data(0x35);
  ili9341_write_data(0x3E);

  ili9341_write_command(0xC7);  //LCD_CMD_VCOM_CTRL2
  ili9341_write_data(0xbe);

  ili9341_write_command(0xB1);  //LCD_CMD_FRAME_CTRL
  ili9341_write_data(0x00);
  ili9341_write_data(0x1b);

  ili9341_write_command(0xB6);  //LCD_CMD_FRAME_CTRL
  ili9341_write_data(0x0A);
  ili9341_write_data(0x82);
  ili9341_write_data(0x27);
  ili9341_write_data(0x00);

  ili9341_write_command(0xB7);  //LCD_CMD_ENTRY_MODE
  ili9341_write_data(0x07);

  ili9341_write_command(0x3A);  //LCD_CMD_PIXEL_FORMAT
  ili9341_write_data(0x55);//16bit

  ili9341_write_command(0x2a);  //
  ili9341_write_data(0x00);
  ili9341_write_data(0x00);
  ili9341_write_data(0x00);
  ili9341_write_data(0xef);

  ili9341_write_command(0x2b);  //
  ili9341_write_data(0x00);
  ili9341_write_data(0x00);
  ili9341_write_data(0x01);
  ili9341_write_data(0x3f);


  ili9341_write_command(0x13);  //normal mode

//  ili9341_write_command(0x39);  //idle mode

  
  ili9341_write_command(0x36);  //LCD_CMD_MEMACCESS_CTRL
  ili9341_write_data(0b11011100);

  
  ili9341_write_command(0x11);  //LCD_CMD_SLEEPOUT

  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  __delay_ms(10);
  
  
  ili9341_write_command(0x29);  //display on

  __delay_ms(10);
  __delay_ms(10);

  ili9341_write_command(0x2c);  //memory write
  

  counter=0;
  while(counter<76800)//76799)) //240*320 didn't work ??? //clear screen
  {
  counter++;
  ili9341_write_data(0x00); 
  ili9341_write_data(0x00);
  }

}
