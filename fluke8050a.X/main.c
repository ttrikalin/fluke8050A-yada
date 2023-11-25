#include  "xc8.h"
#include  "fonts/fonts.h"
#include  "functions.h"
#include  "setup.h"


// I'm not using mplab....
// using following line to compile for PIC18F46K22
// xc8 --CHIP=18F46K22 fluke8050a_ili9341_pic18f46k22.c

// using following line to program PIC18F46K22 with a PICkit2
// pk2cmd -pPIC18F46K22 -ffluke8050a_ili9341_pic18f46k22.hex -M -J -R


// Horizontal is 240 (short)
// Vertical is 320 (long)



#ifndef _XTAL_FREQ
#define _XTAL_FREQ 32000000

#endif


#define ili9341_cs        PORTCbits.RC7
#define ili9341_reset     PORTCbits.RC6
#define ili9341_dc        PORTDbits.RD2
//    ili9341_soi       pin 24 SDO1
//    ili9341_clk       pin 18 SCK1

#define fluke8050a_ST0      PORTCbits.RC1
#define fluke8050a_ST1      PORTCbits.RC0
#define fluke8050a_ST2      PORTAbits.RA6
#define fluke8050a_ST3      PORTAbits.RA7
#define fluke8050a_ST4      PORTEbits.RE2

#define fluke8050a_HV       PORTCbits.RC4

#define fluke8050a_FUNC_A   PORTCbits.RC2
#define fluke8050a_FUNC_B   PORTAbits.RA4
#define fluke8050a_FUNC_C   PORTDbits.RD0
//#define fluke8050a_FUNC_D   not used


#define fluke8050a_W      PORTBbits.RB1
#define fluke8050a_X      PORTBbits.RB2
#define fluke8050a_Y      PORTBbits.RB3
#define fluke8050a_Z      PORTBbits.RB4
#define fluke8050a_DP     PORTDbits.RD3

#define fluke8050a_RNG_A    PORTAbits.RA1
#define fluke8050a_RNG_B    PORTAbits.RA2
#define fluke8050a_RNG_C    PORTAbits.RA3


// VGA color palette
#define VGA_BLACK   0x0000
#define VGA_WHITE   0xFFFF
#define VGA_RED     0xF800
#define VGA_GREEN   0x0400
#define VGA_BLUE    0x001F
#define VGA_SILVER    0xC618
#define VGA_GRAY    0x8410
#define VGA_MAROON    0x8000
#define VGA_YELLOW    0xFFE0
#define VGA_OLIVE   0x8400
#define VGA_LIME    0x07E0
#define VGA_AQUA    0x07FF
#define VGA_TEAL    0x0410
#define VGA_NAVY    0x0010
#define VGA_FUCHSIA   0xF81F
#define VGA_PURPLE    0x8010
#define VGA_TRANSPARENT 0xFFFFFFFF


#define foreground_colour VGA_YELLOW
#define background_colour VGA_BLACK

#define ac_colour     VGA_WHITE

#define volts_colour    VGA_LIME
#define amps_colour     VGA_RED
#define ohms_colour     VGA_YELLOW
#define siemens_colour    VGA_WHITE
#define db_colour     VGA_AQUA

#define range_colour    VGA_WHITE



#define char_pixel_width  30
#define char_pixel_height 67
#define char_bytes_height 9

#define decimal_point_pixel_width 12

#define blank_width     12

#define small_char_pixel_width  21
#define small_char_pixel_height 36
#define small_char_bytes_height 5

#define small_blank_width 2


#define range_char_pixel_width  16
#define range_char_pixel_height 17
#define range_char_bytes_height 3

#define range_blank_width 2


typedef unsigned long uint32;
typedef unsigned int  uint16;
typedef unsigned char uint8;
typedef unsigned char uint_8;


unsigned long  counter,counter_bcd;

unsigned int   number_display_x,number_display_y;
unsigned int   number_display_foreground,number_display_background;

unsigned int  ten_thousands,thousands,hundreds,tens,ones,display_bcd;
unsigned int  range,range_old,function,hv,db_scanning_mode;




void mc_pin_setup(void){
    //no analog
    //  ANSELA  = 0;
    //  ANSELB  = 0;
    //  ANSELC  = 0;
    //  ANSELD  = 0;
    //  ANSELE  = 0;
    
    // A/D converter is off  
    ADCON0 = 0x00; 
    
    // All inputs digital (may need an analog input for tone)
    ADCON1 = 0b00001111; 
    ADCON2 = 0x00; 
  
    OSCCON  = 0b01110000; //8MHz
    OSCTUNE = 0b01000000; //use PLL x4  -> 32MHz 
  
  
    TRISA = 0xff;   /*all bits input */
    TRISB = 0xff;   /*all bits input */
    TRISC = 0b00010111; 
    TRISD = 0b11111011;
    TRISE = 0xff;   /*all bits input */
  
  ili9341_cs = 1;

  // Synchronous serial port that communicates with the display driver 
  SSPSTAT  = 0;  // status register 
  SSPCON1  = 0b00110000; // SPI host mode -- FOSC/4; idle state clock HIGH; enable serial port with SCK, SDOO, SDI and bar_SS as serial port pins 
  
  number_display_foreground = foreground_colour;
  number_display_background = background_colour;  
}

void ili9341_write_command (unsigned int data)
{
	ili9341_dc = 0;	
	ili9341_cs = 0;
	
	SSPBUF = (unsigned char) data;
	while(!SSPIF);
	SSPIF=0;
	
	ili9341_cs = 1;
}



// send 8 bits  of data 
void ili9341_write_data (unsigned int data)
{
	unsigned int loop;
	ili9341_dc = 1;	
	ili9341_cs = 0;
	
	SSPBUF = (unsigned char) data;
	while(!SSPIF);
	SSPIF = 0;

	ili9341_cs = 1;
}


// send 16 bits  of data 
void ili9341_write_data16 (unsigned int data)
{
	unsigned int loop;
	ili9341_dc = 1;	
	ili9341_cs = 0;
	
	SSPBUF=	data>>8;
	while(!SSPIF);
	SSPIF=0;

	SSPBUF = (unsigned char) data;
	while(!SSPIF);
	SSPIF=0;	
	
	ili9341_cs = 1;
}


void print_number(unsigned int digit)
	{
	unsigned int 	row=0;					//	0 -> (character_width-1)
	unsigned int 	column_pixel=0;			//	0 -> (character_height-1)
//	unsigned int 	column_byte_index=0;	//	which byte font pixel is located
	unsigned int	column_byte_bit=0;		//	0 -> 7 which pixel in column_byte_index
	unsigned int	work_column_byte;		//	copy of column_byte_index (need to play with to get past unused bits in font table)

	unsigned int	lookup	=	digit * char_pixel_width * char_bytes_height;		// 	how for into font table our byte is

		while (row<char_pixel_width)
			{
	
			column_byte_bit = 	0;
			column_pixel	=	0;

			while	(column_pixel<char_pixel_height)
				{
					work_column_byte	=	fontBitmaps[lookup++];
					column_byte_bit	=	0;

					while	((column_byte_bit<8)&&(column_pixel<(char_pixel_height+1)))
					{
					if (work_column_byte & 0x80)
							ili9341_write_data16(number_display_foreground);
						else
							ili9341_write_data16(number_display_background);

					work_column_byte = work_column_byte<<1;
					column_byte_bit = 	column_byte_bit +1;
					column_pixel	=	column_pixel+1;
					}
		
				}
		
			row++;

			}	

	}

void print_blank_columns(unsigned int blank_columns)
	{
	unsigned int 	work_counter=blank_columns*(char_pixel_height+1);//(blank_columns*(char_pixel_height-1);
	
	while(work_counter>0)
		{
		ili9341_write_data16(number_display_background);
		work_counter=work_counter-1;		
		}

	}

void print_decimal_point()
	{
	unsigned int 	row=0;					//	0 -> (character_width-1)
	unsigned int 	column_pixel=0;			//	0 -> (character_height-1)
//	unsigned int 	column_byte_index=0;	//	which byte font pixel is located
	unsigned int	column_byte_bit=0;		//	0 -> 7 which pixel in column_byte_index
	unsigned int	work_column_byte;		//	copy of column_byte_index (need to play with to get past unused bits in font table)

	unsigned int	lookup	=	0;			// 	how for into font table our byte is

		while (row<decimal_point_pixel_width)
			{
	
			column_byte_bit = 	0;
			column_pixel	=	0;

			while	(column_pixel<char_pixel_height)
				{
					work_column_byte	=	fontDecimalPoint[lookup++];
					column_byte_bit	=	0;

					while	((column_byte_bit<8)&&(column_pixel<(char_pixel_height+1)))
					{
					if (work_column_byte & 0x80)
							ili9341_write_data16(number_display_foreground);
						else
							ili9341_write_data16(number_display_background);

					work_column_byte = work_column_byte<<1;
					column_byte_bit = 	column_byte_bit +1;
					column_pixel	=	column_pixel+1;
					}
		
				}
		
			row++;

			}	

	}

void print_small_characters(unsigned int digit)
	{
	unsigned int 	row=0;					//	0 -> (character_width-1)
	unsigned int 	column_pixel=0;			//	0 -> (character_height-1)
//	unsigned int 	column_byte_index=0;	//	which byte font pixel is located
	unsigned int	column_byte_bit=0;		//	0 -> 7 which pixel in column_byte_index
	unsigned int	work_column_byte;		//	copy of column_byte_index (need to play with to get past unused bits in font table)

	unsigned int	lookup	=	digit * small_char_pixel_width * small_char_bytes_height;		// 	how for into font table our byte is

		while (row<small_char_pixel_width)
			{
	
			column_byte_bit = 	0;
			column_pixel	=	0;

			while	(column_pixel<small_char_pixel_height)
				{
					work_column_byte	=	fontSmallBitmaps[lookup++];
					column_byte_bit	=	0;

					while	((column_byte_bit<8)&&(column_pixel<(small_char_pixel_height+1)))
					{
					if (work_column_byte & 0x80)
							ili9341_write_data16(number_display_foreground);
						else
							ili9341_write_data16(number_display_background);

					work_column_byte = work_column_byte<<1;
					column_byte_bit = 	column_byte_bit +1;
					column_pixel	=	column_pixel+1;
					}
		
				}
		
			row++;

			}	

	}	
	
void print_small_blank_columns(unsigned int blank_columns)
	{
	unsigned int 	work_counter=blank_columns*(small_char_pixel_height+1);
	
	while(work_counter>0)
		{
		ili9341_write_data16(number_display_background);
		work_counter=work_counter-1;		
		}

	}	

void print_range_number(unsigned int digit)
	{
	unsigned int 	row=0;					//	0 -> (character_width-1)
	unsigned int 	column_pixel=0;			//	0 -> (character_height-1)
//	unsigned int 	column_byte_index=0;	//	which byte font pixel is located
	unsigned int	column_byte_bit=0;		//	0 -> 7 which pixel in column_byte_index
	unsigned int	work_column_byte;		//	copy of column_byte_index (need to play with to get past unused bits in font table)

	unsigned int	lookup	=	digit * range_char_pixel_width * range_char_bytes_height;		// 	how for into font table our byte is

		while (row<range_char_pixel_width)
			{
	
			column_byte_bit = 	0;
			column_pixel	=	0;

			while	(column_pixel<range_char_pixel_height)
				{
					work_column_byte	=	fontRange[lookup++];
					column_byte_bit	=	0;

					while	((column_byte_bit<8)&&(column_pixel<(range_char_pixel_height+1)))
					{
					if (work_column_byte & 0x80)
							ili9341_write_data16(number_display_foreground);
						else
							ili9341_write_data16(number_display_background);

					work_column_byte = work_column_byte<<1;
					column_byte_bit = 	column_byte_bit +1;
					column_pixel	=	column_pixel+1;
					}
		
				}
		
			row++;

			}	

	}


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

  
void main(void){
  mc_pin_setup();
  ili9341_setup();
  
  while(1){



  while(!fluke8050a_ST0); 
  while(fluke8050a_ST0);    
  range_old = range;
  db_scanning_mode=0;

  
  
  range = (unsigned int) ((fluke8050a_RNG_C<<2)|(fluke8050a_RNG_B<<1)|fluke8050a_RNG_A);
  
  function = (unsigned int) ((fluke8050a_FUNC_C<<2)|(fluke8050a_FUNC_B<<1)|(fluke8050a_FUNC_A));

  if (((function&0x06)==0x06)&&(!(range>>1))) //extra set of strobe pulse when in dB REF Z mode 
    {
    db_scanning_mode=1;
    if (range_old!=range) //three sets of stobe pulse when first going in REF Z- disregard first two stobe sets
      {         //could get fancy and grab REF Z value and display it
      while(!fluke8050a_ST0); 
      while(fluke8050a_ST0);
      }
    while(!fluke8050a_ST0); 
    while(fluke8050a_ST0);
    }

  ten_thousands = (unsigned int) ((fluke8050a_DP<<4)|(fluke8050a_W<<3)|(fluke8050a_X<<2)|(fluke8050a_Y<<1)|(fluke8050a_Z));//rel sym bit4, number bits3-0

  hv  = fluke8050a_HV;    //read hv pin at any stobe
  
  while(!fluke8050a_ST1);
  while(fluke8050a_ST1);
  thousands = (unsigned int) ((fluke8050a_DP<<4)|(fluke8050a_W<<3)|(fluke8050a_X<<2)|(fluke8050a_Y<<1)|(fluke8050a_Z));
  
  while(!fluke8050a_ST2);
  while(fluke8050a_ST2);
  hundreds = (unsigned int) ((fluke8050a_DP<<4)|(fluke8050a_W<<3)|(fluke8050a_X<<2)|(fluke8050a_Y<<1)|(fluke8050a_Z));
  
  while(!fluke8050a_ST3);
  while(fluke8050a_ST3);
  tens = (unsigned int) ((fluke8050a_DP<<4)|(fluke8050a_W<<3)|(fluke8050a_X<<2)|(fluke8050a_Y<<1)|(fluke8050a_Z));
  
  while(!fluke8050a_ST4);
  while(fluke8050a_ST4);
  ones = (unsigned int) ((fluke8050a_DP<<4)|(fluke8050a_W<<3)|(fluke8050a_X<<2)|(fluke8050a_Y<<1)|(fluke8050a_Z));


  if (counter>20000) {
      counter=0;
  }


  number_display_background = background_colour;
  
  number_display_x  = 20;

  ili9341_write_command(0x2a);  //column address set
  ili9341_write_data16(number_display_x);
  ili9341_write_data16(number_display_x+char_pixel_height);
  
  ili9341_write_command(0x2b);  //page address set
  ili9341_write_data16(5);
  ili9341_write_data16(319);  
  
  ili9341_write_command(0x2c);  //memory write

  
  

  if  (ten_thousands&0x04)
    {
    print_number(0x10); //plus sign
    }
  else if
    (ten_thousands&0x08)
      {
      print_number(0x11); //minus sign
      }
    else
      print_blank_columns(char_pixel_width);


  print_blank_columns(blank_width);
  if (((ten_thousands&0x01)==0) &&  ((hundreds&0x10)|(tens&0x10)|(ones&0x10)|db_scanning_mode))
    print_blank_columns(char_pixel_width);  //blank leading zeros
  else
    print_number(ten_thousands&0x01); 


  print_blank_columns(blank_width);
  if  (thousands&0x10)
    {
    print_decimal_point();
    print_blank_columns(blank_width);
    }
    
    
  if ((ten_thousands&0x01)==0&&(thousands&0x0f)==0&&((tens&0x10)|(ones&0x10)))  
    print_blank_columns(char_pixel_width);  //blank leading zeros
  else
    print_number(thousands&0x0f);

  print_blank_columns(blank_width);
  if  (hundreds&0x10)
    {
    print_decimal_point();
    print_blank_columns(blank_width);
    }
    
  if ((ten_thousands&0x01)==0&&(thousands&0x0f)==0&&(hundreds&0x0f)==0&&(ones&0x10))  
    print_blank_columns(char_pixel_width);  //blank leading zeros
  else  
    print_number(hundreds&0x0f);
    
  print_blank_columns(blank_width);
  if  (tens&0x10)
    {
    print_decimal_point();
    print_blank_columns(blank_width);
    }
  print_number(tens&0x0f);


  if  ((ones<0x19)||(tens<0x19))  //hack to not overwrite right side of display during error
  {
  print_blank_columns(blank_width);
  if  (ones&0x10)
    {
    print_decimal_point();
    print_blank_columns(blank_width);
    }
  print_number(ones&0x0f);
  }

  if  (db_scanning_mode)
    print_blank_columns(char_pixel_width);  //hack to clear right side when no decimal point
  
  
  number_display_x  = 95;

  ili9341_write_command(0x2a);  //column address set
  ili9341_write_data16(number_display_x);
  ili9341_write_data16(number_display_x+small_char_pixel_height);
  
  ili9341_write_command(0x2b);  //page address set
  ili9341_write_data16(180);
  ili9341_write_data16(319);  
  
  ili9341_write_command(0x2c);  //memory write

//function REL  mA  V AC/DC
//  bx00x=ohms
//  bxxx1=AC
//  bx01x=V
//  bx10x=mA
//  bx11x=dB
//  b1xxx=not REL
  
//Range
// 0=20 Mohm  
// 1=200 nS
// 2=2 mS
// 3=200 ohm,uA,mV
// 4=200 kohm,mA,V
// 5=20 kohm,mA,V
// 6=2 kohm,mA,V
// 7=2000 kohm,mA,V 
  
  
  if ((function&0x06)==0x02)  // if function = volts
  {
    number_display_foreground = volts_colour;
    if ((range)==0x03)
      print_small_characters(9);  //m
    else
      print_small_blank_columns(small_char_pixel_width);  //_

    print_small_characters(6);  //V
    
    if (function&0x01)
      {
      number_display_foreground = ac_colour;
      print_small_characters(0);  //A
      }
    else
      print_small_characters(3);  //D

    print_small_characters(2);  //C
    number_display_foreground = volts_colour;
  }


  if ((function&0x06)==0x04)  // if function = amps
  {
    number_display_foreground = amps_colour;
  
    if ((range)==0x03)
      print_small_characters(12); //u
    else
      print_small_characters(9);  //m

    print_small_characters(0);  //a

    if (function&0x01)
      {
      number_display_foreground = ac_colour;
      print_small_characters(0);  //A
      }
    else
      print_small_characters(3);  //D

    print_small_characters(2);  //C
    number_display_foreground = amps_colour;
  }

  
  if (((function&0x06)==0x00)     && (range!=1)&&(range!=2))  // if function = ohms
  {
    number_display_foreground = ohms_colour;
    print_small_blank_columns(small_char_pixel_width);  //_
    print_small_blank_columns(small_char_pixel_width);  //_ 
    if ((range)==0x03)
      print_small_blank_columns(small_char_pixel_width);  //_
    else  if  ((range)==0x00)
        print_small_characters(4);  //M
      else
        print_small_characters(8);  //k
    print_small_characters(11); //omega
  } 
  

  if (((function&0x06)==0x00)     && ((range==1)||(range==2)))  // if function = Siemens
  {
    number_display_foreground = siemens_colour;
    print_small_blank_columns(small_char_pixel_width);  //_
    print_small_blank_columns(small_char_pixel_width);  //_ 
    if ((range)==0x02)
        print_small_characters(9);  //m
    else
        print_small_characters(10); //n
    print_small_characters(5);  //S
  }   

  if ((function&0x06)==0x06)  // if function = dB 
  {
  
    if  (db_scanning_mode)
      {
      number_display_foreground = db_colour;
      print_small_blank_columns(small_char_pixel_width);  //_
      print_small_blank_columns(small_char_pixel_width);  //_ 
      print_small_blank_columns(small_char_pixel_width);  //_ 
      print_small_characters(11); //omega
      }

    else
      {
      number_display_foreground = db_colour;
      print_small_blank_columns(small_char_pixel_width);  //_
      print_small_blank_columns(small_char_pixel_width);  //_ 
      print_small_characters(7);  //d
      print_small_characters(1);  //B
      }
  }   
  
  
  number_display_x  = 95;

  ili9341_write_command(0x2a);  //column address set
  ili9341_write_data16(number_display_x);
  ili9341_write_data16(number_display_x+range_char_pixel_height);
  
  ili9341_write_command(0x2b);  //page address set
  ili9341_write_data16(100);
  ili9341_write_data16(319);  
  
  ili9341_write_command(0x2c);  //memory write


  // if function = dB don't show range, should show
  if ((function&0x06)==0x06) {  
    print_range_number(0);//_
    print_range_number(0);//_
    print_range_number(0);//_
    print_range_number(0);//_
  } else { 
    if (range==0) { //20 Mohm
      if  ((function&0b0110)==0) {
        print_range_number(0);//_
        print_range_number(0);//_
        print_range_number(2);//2
        print_range_number(1);//0
      }
      else {
        print_range_number(0);//_
        print_range_number(0);//_
        print_range_number(0);//_
        print_range_number(0);//_
      }
    }
    if (range==1) { //200 nS
      if  ((function&0b0110)==0) {
        print_range_number(0);//_
        print_range_number(2);//2
        print_range_number(1);//0
        print_range_number(1);//0
      } else {
        print_range_number(0);//_
        print_range_number(0);//_
        print_range_number(0);//_
        print_range_number(0);//_
      }
    }
      
    if (range==2) { //2 mS
      if  ((function&0b0110)==0) {
        print_range_number(0);//_
        print_range_number(0);//_
        print_range_number(0);//_
        print_range_number(2);//2
      } else {
        print_range_number(0);//_
        print_range_number(0);//_
        print_range_number(0);//_
        print_range_number(0);//_
      }
    }
    if (range==3) { //200 ohm,uA,mV
      print_range_number(0);//_
      print_range_number(2);//2
      print_range_number(1);//0
      print_range_number(1);//0
    }

    if (range==4) { //200 kohm,mA,V
      print_range_number(0);//_
      print_range_number(2);//2
      print_range_number(1);//0
      print_range_number(1);//0
    }

    if (range==5) { //20 kohm,mA,V
      print_range_number(0);//_
      print_range_number(0);//_
      print_range_number(2);//2
      print_range_number(1);//0
    }

    if (range==6) { //2 kohm,mA,V
      print_range_number(0);//_
      print_range_number(0);//_
      print_range_number(0);//_
      print_range_number(2);//2
    }

    if (range==7) { //2000 kohm,mA,V
      print_range_number(2);//2
      print_range_number(1);//0
      print_range_number(1);//0
      print_range_number(1);//0
    }

  } 


  number_display_x  = 70;

  ili9341_write_command(0x2a);  //column address set
  ili9341_write_data16(number_display_x);
  ili9341_write_data16(number_display_x+range_char_pixel_height);
  
  ili9341_write_command(0x2b);  //page address set
  ili9341_write_data16(272);
  ili9341_write_data16(319);  
  
  ili9341_write_command(0x2c);  //memory write

  //if (function&0x08)  // /REL <button>
  if (ten_thousands&0x10) // REL <display>
      {
      print_range_number(6);//R
      print_range_number(3);//E
      print_range_number(5);//L
      }
    else
      {
      print_range_number(0);//_
      print_range_number(0);//_
      print_range_number(0);//_
      }
        
      
  number_display_x  = 45;
      
  ili9341_write_command(0x2a);  //column address set
  ili9341_write_data16(number_display_x);
  ili9341_write_data16(number_display_x+range_char_pixel_height);
  
  ili9341_write_command(0x2b);  //page address set
  ili9341_write_data16(272);
  ili9341_write_data16(319);  
  
  ili9341_write_command(0x2c);  //memory write      

  if (hv) // HV  ***do lighting bolt?
      {
      print_range_number(4);//H
      print_range_number(7);//V
      }
    else  
      {
      print_range_number(0);//_
      print_range_number(0);//_
      }

  counter++;

  }

}


