#include   <htc.h> 
#include "font.inc"
//#include	"EEP.h"		//needed for saving state when power is off

// I'm not using mplab....
// using following line to compile for PIC18F46K22
// xc8 --CHIP=18F46K22 fluke8050a_ili9341_pic18f46k22.c

// using following line to program PIC18F46K22 with a PICkit2
// pk2cmd -pPIC18F46K22 -ffluke8050a_ili9341_pic18f46k22.hex -M -J -R


// Horizontal is 240 (short)
// Vertical is 320 (long)


#pragma config WDTEN = OFF, FOSC =INTIO67,PLLCFG=ON, LVP = OFF ,PWRTEN  =ON	

#ifndef _XTAL_FREQ
 // Unless specified elsewhere, 64MHz system frequency is assumed
#define _XTAL_FREQ 64000000

#endif


#define	ili9341_cs				PORTCbits.RC7
#define	ili9341_reset			PORTCbits.RC6
#define	ili9341_dc				PORTDbits.RD2
//		ili9341_soi				pin 24 SDO1
//		ili9341_clk				pin 18 SCK1

#define	fluke8050a_ST0			PORTCbits.RC1
#define	fluke8050a_ST1			PORTCbits.RC0
#define	fluke8050a_ST2			PORTAbits.RA6
#define	fluke8050a_ST3			PORTAbits.RA7
#define	fluke8050a_ST4			PORTEbits.RE2

#define	fluke8050a_HV			PORTCbits.RC4

#define	fluke8050a_FUNC_A		PORTCbits.RC2
#define	fluke8050a_FUNC_B		PORTAbits.RA4
#define	fluke8050a_FUNC_C		PORTDbits.RD0
//#define	fluke8050a_FUNC_D		not used


#define	fluke8050a_W			PORTBbits.RB1
#define	fluke8050a_X			PORTBbits.RB2
#define	fluke8050a_Y			PORTBbits.RB3
#define	fluke8050a_Z			PORTBbits.RB4
#define	fluke8050a_DP			PORTDbits.RD3

#define	fluke8050a_RNG_A		PORTAbits.RA1
#define	fluke8050a_RNG_B		PORTAbits.RA2
#define	fluke8050a_RNG_C		PORTAbits.RA3

#define	test_out1				PORTEbits.RE1



// VGA color palette
#define VGA_BLACK		0x0000
#define VGA_WHITE		0xFFFF
#define VGA_RED			0xF800
#define VGA_GREEN		0x0400
#define VGA_BLUE		0x001F
#define VGA_SILVER		0xC618
#define VGA_GRAY		0x8410
#define VGA_MAROON		0x8000
#define VGA_YELLOW		0xFFE0
#define VGA_OLIVE		0x8400
#define VGA_LIME		0x07E0
#define VGA_AQUA		0x07FF
#define VGA_TEAL		0x0410
#define VGA_NAVY		0x0010
#define VGA_FUCHSIA		0xF81F
#define VGA_PURPLE		0x8010
#define VGA_TRANSPARENT	0xFFFFFFFF


#define foreground_colour	VGA_YELLOW
#define background_colour	VGA_BLACK

#define ac_colour			VGA_YELLOW

#define volts_colour		VGA_LIME
#define amps_colour			VGA_RED
#define ohms_colour			VGA_YELLOW
#define siemens_colour		VGA_SILVER
#define db_colour			VGA_BLUE

#define scaled_colour		VGA_WHITE
//#define max_colour			VGA_WHITE
//#define min_colour			VGA_WHITE


#define range_colour		VGA_WHITE

#define	scaled_selected_colour	VGA_BLUE

#define char_pixel_width	30
#define char_pixel_height	67
#define char_bytes_height	9

#define decimal_point_pixel_width	12

#define	blank_width			12

#define small_char_pixel_width	21
#define small_char_pixel_height	36
#define small_char_bytes_height	5

#define	small_blank_width	2


#define range_char_pixel_width	16
#define range_char_pixel_height	17
#define range_char_bytes_height	3

#define	range_blank_width	2


#define minifont_char_pixel_width	20//23//19
#define minifont_char_pixel_height	21//23//22
#define minifont_char_bytes_height	3

#define	minifont_blank_width	0
#define minifont_decimal_point_pixel_width	4





#define	display1_offset	40
#define	display2_offset	160

#define	main_display_x_pos	319-(6*char_pixel_width)-(6*blank_width)-decimal_point_pixel_width -2

#define	display_function_x_pos	319-(4*minifont_char_pixel_width)//276

#define	display_small_numbers_x_pos	105//f80

#define	display_range_x_pos000	0



typedef unsigned long	uint32;
typedef unsigned int	uint16;
typedef unsigned char	uint8;
typedef unsigned char	uint_8;


unsigned long	 counter,counter_bcd;
unsigned long	 counter_minutes=0,counter_hundredth_seconds=0;
unsigned int	counter_minutes_tens='0';
unsigned int	counter_minutes_ones='0';
unsigned int	counter_hours_tens='0';
unsigned int	counter_hours_ones='0';

unsigned int	 number_display_x,number_display_y;
unsigned int	 number_display_foreground,number_display_background;


unsigned int	ten_thousands,thousands,hundreds,tens,ones;
unsigned int	ten_thousands_old,thousands_old,hundreds_old,tens_old,ones_old;
unsigned int	range,range_old,function,function_old,hv,db_scanning_mode;
unsigned int	rel,rel_old;


unsigned int	scaled_ten_thousands,scaled_thousands,hundreds,scaled_tens,ones,scaled_scaled_decimal;
unsigned long	actual_value;
unsigned long	scaled_value;
unsigned int	scaled_decimals999;
unsigned long	scaled_value_bcd;

unsigned int	scaled_multiplier;

unsigned int	scaled_ten_thousands,scaled_thousands,scaled_hundreds,scaled_tens,scaled_ones;


unsigned int	actual_range,actual_range_old;
unsigned int	scaled_range;
unsigned long	actual_function,actual_function_old;
unsigned long	scaled_function;


unsigned long	actual_bcd;
unsigned int	actual_decimals;
unsigned int	scaled_decimals;
unsigned int	actual_sign; //00=nul 11=+ 10 =-
unsigned long	actual_bcd;
signed long		actual_bin;
unsigned int	scaled_sign; //00=nul 11=+ 10 =-
unsigned long	scaled_bcd;
signed long		scaled_bin;
unsigned long	max_bcd;//,max_bin;
unsigned long	min_bcd;//,min_bin;
unsigned int	max_sign;
unsigned int	min_sign;

unsigned int	rel_new;

unsigned int char_pixel_width000;
unsigned int char_pixel_height000;
unsigned int char_bytes_height000;
unsigned int char_pixel_width_displayed;
unsigned int decimal_point_pixel_width000;	

unsigned int	blank_width000;
unsigned int	font_table;
unsigned int	work_test;

unsigned int	diplay_colour;

unsigned int	scale_mode_volts_dc;
unsigned int	scale_mode_volts_ac;
unsigned int	scale_mode_amps_dc;
unsigned int	scale_mode_amps_ac;
unsigned int	scale_mode_volts_dc_delay_counter;

unsigned int	funtion_new_counter;

unsigned int max_colour = VGA_WHITE;//			VGA_WHITE
unsigned int min_colour = VGA_WHITE;//			VGA_WHITE

unsigned int powerup=1;	//=0 after first power up


unsigned long bcd (unsigned long val)	//found on net somewhere
{
  unsigned long bcdresult = 0; char i;

  for (i = 0; val; i++)
  {
    ((char*)&bcdresult)[i / 2] |= i & 1 ? (val % 10) << 4 : (val % 10) & 0xf;
    val /= 10;
  }
  return bcdresult;

}	

unsigned long bin (unsigned long val)
{
  unsigned long binresult;
  
  
  binresult	= (((val>>16)&0x01)*10000 + ((val>>12)&0x0f)*1000 + ((val>>8)&0x0f)*100 + ((val>>4)&0x0f)*10 + ((val)&0x0f));
  

  return binresult;

}	


void ili9341_write_command (unsigned int data)
{
	ili9341_dc = 0;	
	ili9341_cs = 0;
	
	SSPBUF=	data;
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
	
	SSPBUF=	data;
	while(!SSPIF);
	SSPIF=0;

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

	SSPBUF=	data;
	while(!SSPIF);
	SSPIF=0;	
	
	ili9341_cs = 1;
}

/*
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
*/
	
void print_number000(unsigned int digit)
	{
	unsigned int 	row=0;					//	0 -> (character_width-1)
	unsigned int 	column_pixel=0;			//	0 -> (character_height-1)
//	unsigned int 	column_byte_index=0;	//	which byte font pixel is located
	unsigned int	column_byte_bit=0;		//	0 -> 7 which pixel in column_byte_index
	unsigned int	work_column_byte;		//	copy of column_byte_index (need to play with to get past unused bits in font table)

	if	(font_table==2)
		digit	=	digit+15;
	
	unsigned int	lookup	=	digit * char_pixel_width000 * char_bytes_height000;		// 	how for into font table our byte is


		while (row<char_pixel_width_displayed)
			{
	
			column_byte_bit = 	0;
			column_pixel	=	0;

			while	(column_pixel<char_pixel_height000)
				{
					if	(font_table==1)
						work_column_byte	=	fontBitmaps[lookup++];
					if	(font_table==2)
						work_column_byte	=	minifont[lookup++];
					column_byte_bit	=	0;

					while	((column_byte_bit<8)&&(column_pixel<(char_pixel_height000+1)))
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
	unsigned int 	work_counter=blank_columns*(char_pixel_height000+1);//(blank_columns*(char_pixel_height-1);
	
	while(work_counter>0)
		{
		ili9341_write_data16(number_display_background);
		work_counter=work_counter-1;		
		}

	}
/*
void print_decimal_point000()
	{
	unsigned int 	row=0;					//	0 -> (character_width-1)
	unsigned int 	column_pixel=0;			//	0 -> (character_height-1)
//	unsigned int 	column_byte_index=0;	//	which byte font pixel is located
	unsigned int	column_byte_bit=0;		//	0 -> 7 which pixel in column_byte_index
	unsigned int	work_column_byte;		//	copy of column_byte_index (need to play with to get past unused bits in font table)

	unsigned int	lookup	=	0;			// 	how for into font table our byte is

		while (row<decimal_point_pixel_width000)
			{
	
			column_byte_bit = 	0;
			column_pixel	=	0;

			while	(column_pixel<char_pixel_height)
				{
	//				work_column_byte	=	fontDecimalPoint[lookup++];
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
*/


void	print_decimal_point()
	{

	
	
	
	char_pixel_width_displayed	=	decimal_point_pixel_width000;


	
	
	if	(font_table	==	1)
		{
		print_number000(0x12);	//decimal point pheithcvmjgglkmgfm
		
		char_pixel_width_displayed		=	char_pixel_width;	//hack to restore char width that I destroyed
		}
	if	(font_table	==	2)
		{
		print_number000(99-15);	//decimal point pheiifhlskdflkflfkm
	
	
		char_pixel_width_displayed		=	minifont_char_pixel_width;		
	
		}


	}
	
void	set_up_font_large(void)
	{
	font_table				=	1;
	char_pixel_width000		=	char_pixel_width;
	char_pixel_height000	=	char_pixel_height;
	char_bytes_height000	=	char_bytes_height;
	char_pixel_width_displayed		=	char_pixel_width;
	decimal_point_pixel_width000	=	decimal_point_pixel_width;

	blank_width000	=	blank_width;


	}

void	set_up_minifont(void)
	{
	font_table				=	2;	
	char_pixel_width000		=	minifont_char_pixel_width;
	char_pixel_height000	=	minifont_char_pixel_height;
	char_bytes_height000	=	minifont_char_bytes_height;
	char_pixel_width_displayed		=	minifont_char_pixel_width;
	decimal_point_pixel_width000	=	minifont_decimal_point_pixel_width;

	blank_width000	=	minifont_blank_width;


	}


	
void	print_largefont(unsigned int pos_neg, unsigned long value_bcd,unsigned int decimals,unsigned int horizontal,unsigned vertical)	//value to display, horizontal, vertical
	{
	
	unsigned int	ten_thousands	= 	((value_bcd>>16)&0x0f);
	unsigned int	thousands		=	((value_bcd>>12)&0x0f);
	unsigned int	hundreds		= 	((value_bcd>>8)&0x0f);
	unsigned int	tens			= 	((value_bcd>>4)&0x0f);
	unsigned int	ones			= 	((value_bcd)&0x0f);

	decimals	=	decimals&0xf;

	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(vertical);
	ili9341_write_data16(vertical+char_pixel_height000);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(horizontal);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write	
	

	if	((pos_neg==0b11)&&(font_table	==	1))	//not very elegant
		print_number000(0x10);	//plus sign	
	if	((pos_neg==0b10)&&(font_table	==	1))
		print_number000(0x11);	//minus sign
	if	((pos_neg==0b11)&&(font_table	==	2))
		print_number000('+'-48);	//plus sign
	if	((pos_neg==0b10)&&(font_table	==	2))
		print_number000('-'-48);	//minus sign		
		
		
		
		
	if	(!pos_neg)
		print_blank_columns(char_pixel_width000);


		print_blank_columns(blank_width000);
		//print_number000(font_size,char_pixel_width000,char_pixel_height000,char_bytes_height000,0x0f);	//blank		
	if ((ten_thousands==0) && ((decimals & 0b0111) || !decimals))	//zero blanking
		print_blank_columns(char_pixel_width000);	//blank leading zeros
	else
		print_number000(ten_thousands);


	if	(decimals==0b1111)	//display first dp if error to keep display witdth constant
		{
		print_blank_columns(blank_width000);				
		print_decimal_point();
		}

		
	if	(decimals==0b1000)
		{
		print_blank_columns(blank_width000);			
		print_decimal_point();
		}	
	
	print_blank_columns(blank_width000);									
	if ((ten_thousands==0) && (thousands==0) && ((decimals & 0b0011) || !decimals))
		print_blank_columns(char_pixel_width000);	//blank leading zeros
	else
		print_number000(thousands);
	

	if	(decimals==0b0100)
		{
		print_blank_columns(blank_width000);			
		print_decimal_point();
		}	
		
	print_blank_columns(blank_width000);									
	if ((ten_thousands==0) && (thousands==0) && (hundreds==0) && ((decimals & 0b0001) || !decimals))
		print_blank_columns(char_pixel_width000);	//blank leading zeros
	else
		print_number000(hundreds);

	if	(decimals==0x02)
		{
		print_blank_columns(blank_width000);			
		print_decimal_point();
		}

	print_blank_columns(blank_width000);									
	if ((ten_thousands==0) && (thousands==0) && (hundreds==0) && (tens==0) && ( !decimals))
		print_blank_columns(char_pixel_width000);	//blank leading zeros
	else
		print_number000(tens);


	if	(decimals==0x01)
		{
		print_blank_columns(blank_width000);			
		print_decimal_point();

		}

	print_blank_columns(blank_width000);					
	print_number000(ones);

	if	(!decimals)								//keep display witdth constant
		{
		print_blank_columns(blank_width000);			
		print_blank_columns(decimal_point_pixel_width000);			
		}


	
//	print_blank_columns(blank_width);			//hack to over write right side 
//	print_blank_columns(blank_width);			//if not displaying decimal point
		

	}
	
	
	
	
	
/*	
	
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

*/


/*	
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

*/	
	
	
	
	
// send a character to the UART1 serial port
put_UART( char c)
{
	while ( !TXSTA2bits.TRMT);   // wait while Tx buffer full
	//__delay_us(10);
	TXREG2 = c;
}


// send a null terminated string to the UART2 serial port
void put_string_UART( const char * s)
{
	while( *s)			// loop until *s == '\0' the  end of the string
		put_UART( *s++);

}


// send a character to the lcd screen 
put_lcd( char c)
{
//	while ( !TXSTA2bits.TRMT);   // wait while Tx buffer full
	//__delay_us(10);
//	TXREG2 = c;
	
	
	print_number000(c-48);
	
	
}


// send a null terminated string to the lcd screen
void put_string_lcd( const char * s)
{
	while( *s)			// loop until *s == '\0' the  end of the string
		put_lcd( *s++);

}








void	horizontal_line(unsigned int y_pos)
	{
	unsigned int	loop=0;
	
	
	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(y_pos);
	ili9341_write_data16(y_pos);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(0);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write	

	while	(loop<320)
		{
		ili9341_write_data16(number_display_foreground);
		loop++;
		}
	}


void	horizontal_line_hack(unsigned int y_pos,unsigned int length)
	{
	unsigned int	loop=0;
	
	
	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(y_pos);
	ili9341_write_data16(y_pos);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(0);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write	

	while	(loop<length)
		{
		ili9341_write_data16(number_display_foreground);
		loop++;
		}
	}	
	
	
	
	
void	print_function	(unsigned int display_function_x_pos000, unsigned int function, unsigned int range)
	{
//function REL	mA	V AC/DC
//	bx00x=ohms
//	bxxx1=AC
//	bx01x=V
//	bx10x=mA
//	bx11x=dB
//	b1xxx=not REL
	
//Range
// 0=20 Mohm	
// 1=200 nS
// 2=2 mS
// 3=200 ohm,uA,mV
// 4=200 kohm,mA,V
// 5=20 kohm,mA,V
// 6=2 kohm,mA,V
// 7=2000 kohm,mA,V	

// 8=A			scaled only
	
	//print_blank_columns(char_pixel_width000);//hack
	
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(display_function_x_pos000);
	ili9341_write_data16(319);	
	ili9341_write_command(0x2c);	//memory write	

	
	
	
	
	
	if ((function&0x06)==0x02)	// if function = volts
	{
//		number_display_foreground = volts_colour;
		if ((range)==0x03)
			{
			print_number000('m'-48);	//m
//			put_UART('m');
			}
		else
			{
			print_blank_columns(char_pixel_width000);	//_
			//put_UART(' ');			
			}
		print_number000('V'-48);	//V
//		put_UART('V');
		
		if (function&0x01)
			{
			number_display_foreground = ac_colour;
			print_number000('A'-48);	//A
//			put_UART('A');
			}
		else
			{
			print_number000('D'-48);	//D
//			put_UART('D');
			}
		print_number000('C'-48);	//C
//		put_UART('C');
//		number_display_foreground = volts_colour;
	}


	if ((function&0x06)==0x04)	// if function = amps
	{
//		number_display_foreground = amps_colour;
	
		if ((range)==0x03)
			{
			print_number000(94-15);	//u
//			put_UART('u');
			}
		else if	((range)==8)
			{
			print_blank_columns(char_pixel_width000);	//_
			}
			else	
				{
				print_number000('m'-48);	//m
//				put_UART('m');
				}
		print_number000('A'-48);	//A
//		put_UART('A');

		if (function&0x01)
			{
			number_display_foreground = ac_colour;
			print_number000('A'-48);	//A			
//			put_UART('A');
			}
		else
			{
			print_number000('D'-48);	//D
//			put_UART('D');
			}
		print_number000('C'-48);	//C		
//		put_UART('C');
//		number_display_foreground = amps_colour;
	}

	
	if (((function&0x06)==0x00)     && (range!=1)&&(range!=2))	// if function = ohms
	{
//		number_display_foreground = ohms_colour;
		print_blank_columns(char_pixel_width000);	//_
		print_blank_columns(char_pixel_width000);	//_
		//put_string_UART("  ");
		if ((range)==0x03)
			{
			print_blank_columns(char_pixel_width000);	//_
			//put_UART(' ');
			}
		else	if	((range)==0x00)
				{
				print_number000('M'-48);	//M
//				put_UART('M');
				}
			else
				{
				print_number000('k'-48);	//k
//				put_UART('k');
				}
		print_number000(96-15);	//omega
//		put_string_UART("Ohm");

	}	
	

	if (((function&0x06)==0x00)     && ((range==1)||(range==2)))	// if function = Siemens
	{
//		number_display_foreground = siemens_colour;
		print_blank_columns(char_pixel_width000);	//_
		print_blank_columns(char_pixel_width000);	//_	
		//put_string_UART("  ");
		if ((range)==0x02)
				{
				print_number000('m'-48);	//m
//				put_UART('m');
				}
		else
				{
				print_number000('n'-48);	//n
//				put_UART('n');
				}
		print_number000('S'-48);	//S
//		put_UART('S');
	}		

	if ((function&0x06)==0x06)	// if function = dB	
	{
	
		if	(db_scanning_mode)
			{
//			number_display_foreground = db_colour;
			print_number000('r'-48);		//r
			print_number000('e'-48);	//e	
			print_number000('f'-48);	//f
			print_number000(96-15);		//omega
//			put_string_UART(" Ohm");
			}

		else
			{
//			number_display_foreground = db_colour;
			print_blank_columns(char_pixel_width000);	//_
			//print_blank_columns(char_pixel_width000);	//_	
			print_number000('d'-48);	//d
			print_number000('B'-48);	//B
			print_number000('m'-48);	//m
			
//			put_string_UART(" dB");
			}
	}		






	
	}
	

	
	
void	colour_lookup	(unsigned int function, unsigned	int range)
	{
	if	((function&0x06)==0x02)	// if function = volts
		number_display_foreground = volts_colour;
		
	if ((function&0x06)==0x04)	// if function = amps
		number_display_foreground = amps_colour;
		
	if (((function&0x06)==0x00)     && (range!=1)&&(range!=2))	// if function = ohms
		number_display_foreground = ohms_colour;
		
	if (((function&0x06)==0x00)     && ((range==1)||(range==2)))	// if function = Siemens
		number_display_foreground = siemens_colour;

	if ((function&0x06)==0x06)	// if function = dB	
		number_display_foreground = db_colour;

		
		
		
	
		
		
	}
	

void	print_lcd_max_symbol()
{
		number_display_foreground = max_colour;

	number_display_x=150;	
	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+minifont_char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(70);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write			
		

	//put_string_lcd("mx");
	print_number000(97-15);	//up symbol
	
}

void	clear_lcd_max_symbol()
{
		number_display_foreground = max_colour;

	number_display_x=150;	
	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+minifont_char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(70);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write			
		

	//put_string_lcd("..");
	print_blank_columns(char_pixel_width000);
}




void	print_lcd_min_symbol()
{
		number_display_foreground = max_colour;

	number_display_x=175;	
	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+minifont_char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(70);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write			
		

	//put_string_lcd("in");
	print_number000(98-15);	//down symbol

}

void	clear_lcd_min_symbol()
{
		number_display_foreground = max_colour;

	number_display_x=175;	
	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+minifont_char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(70);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write			
		

	//put_string_lcd("..");
	print_blank_columns(char_pixel_width000);

}	
	
main(void)
{
	 		//no analog
	ANSELA	=	0;
	ANSELB	=	0;
	ANSELC	=	0;
	ANSELD	=	0;
	ANSELE	=	0;

	
	OSCCON	=	0b01110000;	//16MHz
	OSCTUNE	=	0b01000000;	//use pll x4	->64MHz woohoo let's fly
	
	
	TRISA	=	0xff;		/*all bits input */
	TRISB	=	0xff;		/*all bits input */
	TRISC	=	0b00010111;	
	TRISD	=	0b10111011;	// using tx2 output (RD6)
	TRISE	=	0b11111101;		//RE1 is test_out1
	
	ili9341_cs = 1;

	SSP1STAT	=	0;
	SSP1CON1	=	0b00110000;


	SPBRG2				=	138;	//set up uart2 for 115200 @64 MHz
	TXSTA2bits.SYNC		=	0;
	TXSTA2bits.BRGH		=	1;
	BAUDCON2bits.BRG16	=	1;
	TXSTA2bits.TXEN		=	1;
	BAUDCON2bits.CKTXP	= 	1;		//1=invert (direct to pc port)
	RCSTA2bits.SPEN		=	1;
	
	__delay_ms(10);

	put_string_UART( "\x1b[2J");	//clear screen
	put_string_UART( "\x1b[1H");	//home
	put_string_UART( "\x1b[?25l");	//cursor invisible
	put_string_UART("Fluke 8050A....\r\n");
	put_string_UART("\r\n");

	
//	number_display_foreground	=	foreground_colour;
	number_display_background	=	background_colour;	


	
	

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
	
	ili9341_write_command(0x28);	//display off
	ili9341_write_command(0x28);	//display off
	ili9341_write_command(0x28);	//display off
	ili9341_write_command(0x28);	//display off
	
	ili9341_write_command(0x28);	//display off
	__delay_ms(10);
	__delay_ms(10);

	ili9341_write_command(0xCF);	//POWER_CTRLB
	ili9341_write_data(0x00);
	ili9341_write_data(0x83);
	ili9341_write_data(0x30);


	ili9341_write_command(0xED);	//LCD_CMD_POWERON_SEQ_CTRL
	ili9341_write_data(0x64);
	ili9341_write_data(0x03);
	ili9341_write_data(0x12);
	ili9341_write_data(0x81);

	ili9341_write_command(0xE8);	//LCD_CMD_DRV_TIMING_CTRLA
	ili9341_write_data(0x85);
	ili9341_write_data(0x01);
	ili9341_write_data(0x79);

	ili9341_write_command(0xCB);	//LCD_CMD_POWER_CTRLA
	ili9341_write_data(0x39);
	ili9341_write_data(0X2C);
	ili9341_write_data(0x00);
	ili9341_write_data(0x34);
	ili9341_write_data(0x02);

	ili9341_write_command(0xF7);	//LCD_CMD_PUMP_RATIO_CTRL
	ili9341_write_data(0x20);

	ili9341_write_command(0xEA);	//LCD_CMD_DRV_TIMING_CTRLB
	ili9341_write_data(0x00);
	ili9341_write_data(0x00);

	ili9341_write_command(0xC0);	//LCD_CMD_POWER_CTRL1
	ili9341_write_data(0x26);

	ili9341_write_command(0xC1);	//LCD_CMD_POWER_CTRL2
	ili9341_write_data(0x11);

	ili9341_write_command(0xC5);	//LCD_CMD_VCOM_CTRL1
	ili9341_write_data(0x35);
	ili9341_write_data(0x3E);

	ili9341_write_command(0xC7);	//LCD_CMD_VCOM_CTRL2
	ili9341_write_data(0xbe);

	ili9341_write_command(0xB1);	//LCD_CMD_FRAME_CTRL
	ili9341_write_data(0x00);
	ili9341_write_data(0x1b);

	ili9341_write_command(0xB6);	//LCD_CMD_FRAME_CTRL
	ili9341_write_data(0x0A);
	ili9341_write_data(0x82);
	ili9341_write_data(0x27);
	ili9341_write_data(0x00);

	ili9341_write_command(0xB7);	//LCD_CMD_ENTRY_MODE
	ili9341_write_data(0x07);

	ili9341_write_command(0x3A);	//LCD_CMD_PIXEL_FORMAT
	ili9341_write_data(0x55);//16bit

	ili9341_write_command(0x2a);	//
	ili9341_write_data(0x00);
	ili9341_write_data(0x00);
	ili9341_write_data(0x00);
	ili9341_write_data(0xef);

	ili9341_write_command(0x2b);	//
	ili9341_write_data(0x00);
	ili9341_write_data(0x00);
	ili9341_write_data(0x01);
	ili9341_write_data(0x3f);


	ili9341_write_command(0x13);	//normal mode

//	ili9341_write_command(0x39);	//idle mode

	
	ili9341_write_command(0x36);	//LCD_CMD_MEMACCESS_CTRL
	ili9341_write_data(0b11011100);





	
	ili9341_write_command(0x11);	//LCD_CMD_SLEEPOUT

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
	
	
	
	
	
	ili9341_write_command(0x29);	//display on

	__delay_ms(10);
	__delay_ms(10);

	ili9341_write_command(0x2c);	//memory write
	

	counter=0;
	while(counter<76800)			// didn't work ??? //clear screen
	{
	counter++;
	ili9341_write_data(0x00);	
	ili9341_write_data(0x00);
	}

	number_display_foreground	=VGA_YELLOW;
	
	//horizontal_line(0);

	
	horizontal_line(31);//29);
	
	horizontal_line(140);
	horizontal_line(205);
	
	horizontal_line(239);

	while(1)
	{

	test_out1=0;		

	while(!fluke8050a_ST0);	
	while(fluke8050a_ST0);
	


	test_out1=1;		


	function_old		=	function;
	ten_thousands_old	=	ten_thousands;
	thousands_old		=	thousands;
	hundreds_old		=	hundreds;
	tens_old			=	tens;
	ones_old			=	ones;

	db_scanning_mode	=	0;

	
	
	
	actual_range_old = actual_range;
	
	actual_function_old=actual_function;
	

	
	
	


	

	
	
	if (((actual_function&0x06)==0x06)&&(!(actual_range>>1)))	//extra set of strobe pulse when in dB REF Z mode 
		{
		db_scanning_mode=1;
		if (actual_range!=actual_range_old)	//three sets of stobe pulse when first going in REF Z- disregard first two stobe sets
			{								//could get fancy and grab REF Z value and display it
			while(!fluke8050a_ST0);	
			while(fluke8050a_ST0);
			}
		while(!fluke8050a_ST0);	
		while(fluke8050a_ST0);
		}

		
		
		
//	ten_thousands	=(fluke8050a_DP<<4)|(fluke8050a_W<<3)|(fluke8050a_X<<2)|(fluke8050a_Y<<1)|(fluke8050a_Z);//rel sym bit4, neg bit3,   pos bit2,  num bit0
	actual_bcd		=	fluke8050a_Z;
	actual_bcd		=	actual_bcd<<16;
	
	actual_sign		=	(fluke8050a_W<<1)|(fluke8050a_X);	//00=no sign, 11=pos, 10=neg
	rel_old			=	rel;
	rel				=	fluke8050a_DP<<4;
	
	hv	=	fluke8050a_HV;		//read hv pin at any stobe
	
	while(!fluke8050a_ST1);
	while(fluke8050a_ST1);
//	actual_bcd		=	actual_bcd | (fluke8050a_W<<15)|(fluke8050a_X<<14)|(fluke8050a_Y<<13)|(fluke8050a_Z<<12);	//not work, signed 16 ???
	work_test		=	(fluke8050a_W<<3)|(fluke8050a_X<<2)|(fluke8050a_Y<<1)|(fluke8050a_Z<<0);
	actual_bcd		=	actual_bcd |(work_test<<12);
	actual_decimals	=	fluke8050a_DP<<3;
	
	while(!fluke8050a_ST2);
	while(fluke8050a_ST2);
	actual_bcd		=	actual_bcd | (fluke8050a_W<<11)|(fluke8050a_X<<10)|(fluke8050a_Y<<9)|(fluke8050a_Z<<8);
	actual_decimals	=	actual_decimals | fluke8050a_DP<<2;
	
	while(!fluke8050a_ST3);
	while(fluke8050a_ST3);
	actual_bcd		=	actual_bcd | (fluke8050a_W<<7)|(fluke8050a_X<<6)|(fluke8050a_Y<<5)|(fluke8050a_Z<<4);
	actual_decimals	=	actual_decimals | (fluke8050a_DP<<1);
	
	while(!fluke8050a_ST4);
	while(fluke8050a_ST4);
	actual_bcd		=	actual_bcd | (fluke8050a_W<<3)|(fluke8050a_X<<2)|(fluke8050a_Y<<1)|(fluke8050a_Z);
	actual_decimals	=	actual_decimals | (fluke8050a_DP);



	actual_range		=	(fluke8050a_RNG_C<<2)|(fluke8050a_RNG_B<<1)|fluke8050a_RNG_A;
	
	actual_function	=	(fluke8050a_FUNC_C<<2)|(fluke8050a_FUNC_B<<1)|(fluke8050a_FUNC_A);





	
	number_display_background	=	background_colour;
	
	number_display_x	=	display1_offset;

	actual_bin	=	bin(actual_bcd);
		if	(actual_sign==0b10)		//neg
			actual_bin	=	-actual_bin;
		


		
	if	(actual_function!=actual_function_old)
		{
		scale_mode_volts_dc=0;	
		funtion_new_counter=0;

		}		


	if (powerup&&(actual_function==2))		//use scaled mode 3 if in volts dc
		scale_mode_volts_dc=3;				//default scaled mode
	powerup=0;

	if	((actual_range!=actual_range_old)||(rel!=rel_old))
		funtion_new_counter=0;


		

	if	(funtion_new_counter==2)	//wait for display to settle down between selections
		{
		max_bcd		=	scaled_bcd;
		min_bcd		=	scaled_bcd;
		max_sign	=	actual_sign;
		min_sign	=	actual_sign;	
	
	
		}
	

	if	(funtion_new_counter<3)
		funtion_new_counter=funtion_new_counter+1;
	
	

		
	
	//scaled_bin		=	actual_bin * 1;
	//scaled_bin		=	actual_bin * 1.123);		//0.89R sense
	//scaled_bin		=	actual_bin * 1.0034);		//my E3610A
	
	
	if	((scale_mode_volts_dc==1))//&&((actual_bcd&0x0f)<=9))	//need to check for over range too!!!! think here would be good place
		{
	
		if	((actual_range==3)&&(!rel))							//200mV range
			scaled_bin		=	(actual_bin+40) * 0.9285;		//my E3614A has -40mV offset and sense resistor of 1.077 ohms
		else if	((actual_range==6)&&(!rel))						//2V range
				scaled_bin		=	(actual_bin+4) * 0.9285;
			else
				scaled_bin		=	actual_bin * 0.9285;		//bigger ranges or rel    no offset needed

		}
				
				
	if	(scale_mode_volts_dc==2)
		{
	
		if	((actual_range==3)&&(!rel))									//200mV range
			scaled_bin		=	(actual_bin-05) * 1.0034;		//my E3610A has +0.05mV offset and sense resistor of xxxx ohms
		else if	((actual_range==6)&&(!rel))							//2V range
				scaled_bin		=	(actual_bin-0) * 1.0034;
			else
				scaled_bin		=	actual_bin * 1.0034;	//bigger ranges or rel    no offset needed

		}				

		
	if	(scale_mode_volts_dc==3)
		{
	
		if	((actual_range==3)&&(!rel))									//200mV range
			scaled_bin		=	(actual_bin+97) * 1.123;		//my E3617A has -0.97mV offset and sense resistor of xxxx ohms
		else if	((actual_range==6)&&(!rel))							//2V range
				scaled_bin		=	(actual_bin+10) * 1.123;
			else
				scaled_bin		=	actual_bin * 1.123;	//bigger ranges or rel    no offset needed

		}			
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
	scaled_sign	=	0b00;		//start off with no sign		//min max stuff************** broken!!!!!
	if	(actual_sign)			//if pos or neg assume pos
		scaled_sign	=	0b11;

	if	(scaled_bin<0)
		scaled_sign	=	0b10;	//it is neg
		
	if 	(scaled_bin<0)
		scaled_bin=-scaled_bin;
		


	set_up_minifont();//hack
	if	(scaled_sign==0b11)		//pos
		{
		max_sign	=	0b11;	
		if	(scaled_bcd>max_bcd)
			{
			max_bcd		=	scaled_bcd;
			print_lcd_max_symbol();//max_colour	=	VGA_FUCHSIA;	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
			}
		else	
			clear_lcd_max_symbol();//max_colour	=	VGA_WHITE;		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

		if ((min_sign==0b11)&&(scaled_bcd<min_bcd))
			{
			min_bcd		=	scaled_bcd;
			print_lcd_min_symbol();			
			}
		else	
			clear_lcd_min_symbol();			

		}


	if	(scaled_sign==0b10)		//neg
		{
		min_sign	=	0b10;	
		if	(scaled_bcd>min_bcd)
			{
			min_bcd		=	scaled_bcd;
			print_lcd_min_symbol();			
			}
		else	
			clear_lcd_min_symbol();				
			
			
			
		if ((max_sign==0b10)&&(scaled_bcd<max_bcd))
			{
			max_bcd		=	scaled_bcd;
			print_lcd_max_symbol();
			}
		else	
			clear_lcd_max_symbol();			
			
		}

	if	(scaled_sign==0b00)		//no sign
		{
		//max_sign	=	0b00;	
		if	(scaled_bcd>max_bcd)
			{
			max_bcd		=	scaled_bcd;
			print_lcd_max_symbol();
			}
		else	
			clear_lcd_max_symbol();				
			
		if	(scaled_bcd<min_bcd)
			{
			min_bcd		=	scaled_bcd;
			print_lcd_min_symbol();			
			}
		else	
			clear_lcd_min_symbol();				
			
			
	
		}


	scaled_bcd		=	bcd(scaled_bin);



				
	if	((scale_mode_volts_dc==1)|(scale_mode_volts_dc==2))
		scaled_decimals	=	actual_decimals>>1;	
		//scaled_decimals	=	actual_decimals>>0;				//E3617A
		
	else

		scaled_decimals	=	actual_decimals;

		
		
		
		
		
		
		
		
		
	if	(!scale_mode_volts_dc)
		scaled_bcd=	actual_bcd;			//hack for just setting one

	if	(!scale_mode_volts_dc)
		scaled_bin=	actual_bin;			//hack for just setting one



	if	((actual_bcd&0x0f)>9)			//check for orver range
		scaled_bcd=	0xeeeee;			//hack for overload to display properly (shows ">>>>>" on smaller font)



		

//	if	(rel!=rel_old)					
//		{
//		max_bcd		=	scaled_bcd;
//		min_bcd		=	scaled_bcd;
//		max_sign	=	actual_sign;
//		min_sign	=	actual_sign;
//		}




					

		
		

	
	set_up_minifont();						// diplay top line here###################################
	if	(scale_mode_volts_dc)
			{
			colour_lookup(actual_function,actual_range);	
			print_largefont(actual_sign,actual_bcd,actual_decimals,display_small_numbers_x_pos,6);	//value to display, horizontal, vertical
			print_function(display_function_x_pos,actual_function,actual_range);
			}
		else
			{
			ili9341_write_command(0x2a);	//column address set
			ili9341_write_data16(6);
			ili9341_write_data16(6+char_pixel_height000);
	
			ili9341_write_command(0x2b);	//page address set
			ili9341_write_data16(display_small_numbers_x_pos);
			ili9341_write_data16(319);	
			ili9341_write_command(0x2c);	//memory write	

			print_blank_columns(12*char_pixel_width000);

			}
	
	
	
	number_display_foreground = VGA_WHITE;


	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(37);
	ili9341_write_data16(37+char_pixel_height000);
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(0);
//	ili9341_write_data16(319);	
	ili9341_write_command(0x2c);	//memory write
	
	if	(rel)
		print_number000(95-15);	//delta
	else
		print_blank_columns(char_pixel_width000);	//_
		
	
	set_up_font_large();						//main display here************************************************************
	//colour_lookup(scaled_function,actual_function);
	
//	number_display_foreground = scaled_colour;

	if	(scale_mode_volts_dc)
			{
			number_display_foreground = scaled_colour;
			print_largefont(scaled_sign,scaled_bcd,scaled_decimals,main_display_x_pos,display1_offset);	//value to display, horizontal, vertical
			}
		else
			{
			colour_lookup(actual_function,actual_range);
			print_largefont(actual_sign,actual_bcd,actual_decimals,main_display_x_pos,display1_offset);	//value to display, horizontal, vertical
			}
	
	set_up_minifont();
//	colour_lookup(scaled_function,scaled_range);

	


		



	if	(scale_mode_volts_dc)
		{


		scaled_function = 0b0100 | (actual_function&0b001);	//scaled = mA, AC or DC
	
		if	(actual_range==3)			
				scaled_range	=	4;	//if mV show mA
			else
				scaled_range	=	8;	//if  V show A

		}


		
		
		
		
		
		
		
		
	
	if	(scale_mode_volts_dc)
		number_display_foreground = max_colour;
		

	number_display_x=150;	
	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+minifont_char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(0);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write			
		
		
//	print_number000(20);//M
//	print_number000(10);//A
//	print_number000(24);//X
	put_string_lcd("max");

	






	
	
	if	((funtion_new_counter==3)&&((actual_bcd&0x0f)<=9))
		{
	
		print_largefont(max_sign,max_bcd,scaled_decimals,display_small_numbers_x_pos,150);	//value to display, horizontal, vertical
	
		if	(scale_mode_volts_dc)
				print_function(display_function_x_pos,scaled_function,scaled_range);
			else
				print_function(display_function_x_pos,actual_function,actual_range);

		}
	else
		print_blank_columns(13*char_pixel_width000);

	//colour_lookup(scaled_function,scaled_range);	//have to call because AC destorys colour
	if	(scale_mode_volts_dc)
			number_display_foreground = min_colour;
		else
			colour_lookup(actual_function,actual_range);	//have to call because AC destorys colour


	number_display_x=175;	
	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+minifont_char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(0);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write			
		
		
//	print_number000(20);//M
//	print_number000(19);//I
//	print_number000(21);//N

	put_string_lcd("min");


	if	((funtion_new_counter==3)&&((actual_bcd&0x0f)<=9))
		{
			
		print_largefont(min_sign,min_bcd,scaled_decimals,display_small_numbers_x_pos,175);	//value to display, horizontal, vertical

		if	(scale_mode_volts_dc)
				print_function(display_function_x_pos,scaled_function,scaled_range);
			else			
				print_function(display_function_x_pos,actual_function,actual_range);

		}
	else
		print_blank_columns(13*char_pixel_width000);






	number_display_foreground = scaled_selected_colour;

	number_display_x=display1_offset+75;//213;//y

	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+minifont_char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(0);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write
	

	if	(!scale_mode_volts_dc)
		put_string_lcd("      ");
	
	
	if	(scale_mode_volts_dc)
		put_string_lcd("Scaled");

	
	
	
	
	
	
	
	
	
	number_display_x=213;//display1_offset+75;//213;//y

	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+minifont_char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(0);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write
	
	
	if (((actual_function&0x07)==0x02) && (actual_range==0))	// if function = volts dc and 20Mohm switch
		scale_mode_volts_dc_delay_counter	=	scale_mode_volts_dc_delay_counter+1;
			if	(scale_mode_volts_dc_delay_counter>3)
				{
				scale_mode_volts_dc_delay_counter=0;
				scale_mode_volts_dc=scale_mode_volts_dc+1;
					if (scale_mode_volts_dc>4)
						scale_mode_volts_dc=0;
				}
				
				
				
				
//	if (((actual_function&0x06)==0x02) && (actual_range==0))	// if function = volts
	



	if	(!scale_mode_volts_dc)
		put_string_lcd("0:off     ");
	
	
	if	(scale_mode_volts_dc==1)
		{
		print_number000(scale_mode_volts_dc);
		put_string_lcd(": E3614A ");
		}
	
	if	(scale_mode_volts_dc==2)
		{
		print_number000(scale_mode_volts_dc);
		put_string_lcd(": E3610A ");	
		}
	
	if	(scale_mode_volts_dc==3)
		{
		print_number000(scale_mode_volts_dc);
		put_string_lcd(": E3617A ");	
		}
		
	if	(scale_mode_volts_dc==4)
		{
		print_number000(scale_mode_volts_dc);
		put_string_lcd(":100mA/mV");		
		}	





		
	range=scaled_range;//temporary!!!!!!!!!!!!!!!!
	function=scaled_function;//
	
	put_UART(' ');


	
	
	number_display_x	=	display1_offset+75;

	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+minifont_char_pixel_height);
	
//	ili9341_write_command(0x2b);	//page address set
//	ili9341_write_data16(display_function_x_pos,);
//	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write

	//colour_lookup(scaled_function,scaled_range);	//have to call because AC destorys colour
	
	number_display_foreground = scaled_colour;
	
	
	if	(scale_mode_volts_dc)
			{
			number_display_foreground = scaled_colour;
			print_function(display_function_x_pos,scaled_function,scaled_range);
			}
		else
			{
			colour_lookup(actual_function,actual_range);
			print_function(display_function_x_pos,actual_function,actual_range);
			}
	
	
	colour_lookup(actual_function,actual_range);	//have to call because AC destorys colour  temporaryehdbhbkjds

	
	
/*	
	
//function REL	mA	V AC/DC
//	bx00x=ohms
//	bxxx1=AC
//	bx01x=V
//	bx10x=mA
//	bx11x=dB
//	b1xxx=not REL
	
//Range
// 0=20 Mohm	
// 1=200 nS
// 2=2 mS
// 3=200 ohm,uA,mV
// 4=200 kohm,mA,V
// 5=20 kohm,mA,V
// 6=2 kohm,mA,V
// 7=2000 kohm,mA,V	
	
	
	if ((function&0x06)==0x02)	// if function = volts
	{
		number_display_foreground = volts_colour;
		if ((range)==0x03)
			{
			print_small_characters(9);	//m
			put_UART('m');
			}
		else
			{
			print_small_blank_columns(small_char_pixel_width);	//_
			//put_UART(' ');			
			}
		print_small_characters(6);	//V
		put_UART('V');
		
		if (function&0x01)
			{
			number_display_foreground = ac_colour;
			print_small_characters(0);	//A
			put_UART('A');
			}
		else
			{
			print_small_characters(3);	//D
			put_UART('D');
			}
		print_small_characters(2);	//C
		put_UART('C');
		number_display_foreground = volts_colour;
	}


	if ((function&0x06)==0x04)	// if function = amps
	{
		number_display_foreground = amps_colour;
	
		if ((range)==0x03)
			{
			print_small_characters(12);	//u
			put_UART('u');
			}
		else
			{
			print_small_characters(9);	//m
			put_UART('m');
			}
		print_small_characters(0);	//A
		put_UART('A');

		if (function&0x01)
			{
			number_display_foreground = ac_colour;
			print_small_characters(0);	//A
			put_UART('A');
			}
		else
			{
			print_small_characters(3);	//D
			put_UART('D');
			}
		print_small_characters(2);	//C
		put_UART('C');
		number_display_foreground = amps_colour;
	}

	
	if (((function&0x06)==0x00)     && (range!=1)&&(range!=2))	// if function = ohms
	{
		number_display_foreground = ohms_colour;
		print_small_blank_columns(small_char_pixel_width);	//_
		print_small_blank_columns(small_char_pixel_width);	//_
		//put_string_UART("  ");
		if ((range)==0x03)
			{
			print_small_blank_columns(small_char_pixel_width);	//_
			//put_UART(' ');
			}
		else	if	((range)==0x00)
				{
				print_small_characters(4);	//M
				put_UART('M');
				}
			else
				{
				print_small_characters(8);	//k
				put_UART('k');
				}
		print_small_characters(11);	//omega
		put_string_UART("Ohm");

	}	
	

	if (((function&0x06)==0x00)     && ((range==1)||(range==2)))	// if function = Siemens
	{
		number_display_foreground = siemens_colour;
		print_small_blank_columns(small_char_pixel_width);	//_
		print_small_blank_columns(small_char_pixel_width);	//_	
		//put_string_UART("  ");
		if ((range)==0x02)
				{
				print_small_characters(9);	//m
				put_UART('m');
				}
		else
				{
				print_small_characters(10);	//n
				put_UART('n');
				}
		print_small_characters(5);	//S
		put_UART('S');
	}		

	if ((function&0x06)==0x06)	// if function = dB	
	{
	
		if	(db_scanning_mode)
			{
			number_display_foreground = db_colour;
			print_small_blank_columns(small_char_pixel_width);	//_
			print_small_blank_columns(small_char_pixel_width);	//_	
			print_small_blank_columns(small_char_pixel_width);	//_	
			print_small_characters(11);	//omega
			put_string_UART(" Ohm");
			}

		else
			{
			number_display_foreground = db_colour;
			print_small_blank_columns(small_char_pixel_width);	//_
			print_small_blank_columns(small_char_pixel_width);	//_	
			print_small_characters(7);	//d
			print_small_characters(1);	//B
			put_string_UART(" dB");
			}
	}		
*/	
	
	number_display_x	=	3;//display1_offset+75;

//	ili9341_write_command(0x2a);	//column address set
//	ili9341_write_data16(number_display_x);
//	ili9341_write_data16(number_display_x+range_char_pixel_height);
	
//	ili9341_write_command(0x2b);	//page address set
//	ili9341_write_data16(0);
//	ili9341_write_data16(319);	
	
//	ili9341_write_command(0x2c);	//memory write





	set_up_minifont();
	
	
	horizontal_line_hack(2,(4*char_pixel_width000));	
	horizontal_line_hack(3,(4*char_pixel_width000));	
	horizontal_line_hack(4,(4*char_pixel_width000));	
	horizontal_line_hack(5,(4*char_pixel_width000));	
	
	
	
	range=actual_range;//temporary!!!!!!!!!!!!!!!
	function=actual_function;
	number_display_background	=	number_display_foreground;
	number_display_foreground	=	VGA_BLACK;
	
	
	
	
	
	number_display_x	=	6;//y really



	
	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+char_pixel_height000);	
	
	
	
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(display_range_x_pos000);
	ili9341_write_data16(319);	
	ili9341_write_command(0x2c);	//memory write		

	
	if ((function&0x06)==0x06)	// if function = dB	don't show range, should show but am too lazy
		{						// to show properly
		print_number000(0);//_
		print_number000(0);//_
		print_number000(0);//_
		print_number000(0);//_
		}
	else
		{	
		if (range==0)	//20 Mohm
			if	((function&0b0110)==0)
				{
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				print_number000('2'-33);//2
				print_number000('0'-33);//0
				put_string_UART(" (20)");
				}
			else
				{
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				}
		
		if (range==1)	//200 nS
			if	((function&0b0110)==0)
				{
				print_blank_columns(char_pixel_width000);	//_
				print_number000(2);//2
				print_number000(0);//0
				print_number000(0);//0
				put_string_UART(" (200)");
				}
			else
				{
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				}
	
			
		if (range==2)	//2 mS
			if	((function&0b0110)==0)
				{
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				print_number000(2);//2
				put_string_UART(" (2)");
				}
			else
				{
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				print_blank_columns(char_pixel_width000);	//_
				}
			
		if (range==3)	//200 ohm,uA,mV
			{
			print_blank_columns(char_pixel_width000);	//_
			print_number000(2);//2
			print_number000(0);//0
			print_number000(0);//0
			put_string_UART(" (200)");
			}

		if (range==4)	//200 kohm,mA,V
			{
			print_blank_columns(char_pixel_width000);	//_
			print_number000(2);//2
			print_number000(0);//0
			print_number000(0);//0
			put_string_UART(" (200)");
			}

		if (range==5)	//20 kohm,mA,V
			{
			print_blank_columns(char_pixel_width000);	//_
			print_blank_columns(char_pixel_width000);	//_
			print_number000(2);//2
			print_number000(0);//0
			put_string_UART(" (20)");
			}

		if (range==6)	//2 kohm,mA,V
			{
			print_blank_columns(char_pixel_width000);	//_
			print_blank_columns(char_pixel_width000);	//_
			print_blank_columns(char_pixel_width000);	//_
			print_number000(2);//2
			put_string_UART(" (2)");
			}

		if (range==7)	//2000 kohm,mA,V
			{
			print_number000(2);//2
			print_number000(0);//0
			print_number000(0);//0
			print_number000(0);//0
			put_string_UART(" (2000)");
			}

	}	


	
	number_display_background	=	VGA_BLACK;
	number_display_foreground	=	scaled_colour;	

	
	
	
	number_display_x	=	display1_offset+45;


	
	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+range_char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(272);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write

	
	
	
	
	//if (function&0x08)	// /REL <button>
	
	
//	rel_old	=	rel;
//	rel		=	ten_thousands&0x10;
	


/*


	
	if (rel)	// REL <display>
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
				
			
	number_display_x	=	display1_offset+20;
			
	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+range_char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(272);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write			

	if (hv)	// HV  ***do lighting bolt?
			{
			print_range_number(4);//H
			print_range_number(7);//V
			}
		else	
			{
			print_range_number(0);//_
			print_range_number(0);//_
			}

	put_string_UART(" ");						


	
*/	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

//scale value here


/*


	actual_value	=	((ten_thousands&0x01)*10000)+((thousands&0x0f)*1000)+((hundreds&0x0f)*100)+((tens&0x0f)*10)+((ones&0x0f));
	scaled_decimals		=	((thousands>>1)&0x08) |   ((hundreds>>2)&0x04)  |  ((tens>>3)&0x02)  |  ((ones>>4)&0x01);

	number_display_x	=	display2_offset;

	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(5);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write	
	
	if	(rel&&(  (function&0x06)==0x02))	//if in volts and rel show scaled
		{
		scaled_decimals		=	scaled_decimals>>1;
		
		
		if	((ones&0x0f)>9)	//overrange or rel'd out at high range then lower range selected
				scaled_value_bcd	=	0xeeeee;
			else
			{
				//scaled_value=	(actual_value*1.1236);		//0.89R sense

		//		scaled_value=	(actual_value*0.9285);		//my E3614A
				//scaled_value=	(actual_value*1.0034);		//my E3610A
				//scaled_value=	(actual_value*10);	//anything over 5 shows more than display can handle properly
				scaled_value=	(actual_value);

		
				scaled_value_bcd	=	bcd(scaled_value);

			}


		scaled_ten_thousands	= 	((scaled_value_bcd>>16)&0x0f);
		scaled_thousands		=	((scaled_value_bcd>>12)&0x0f);
		scaled_hundreds			= 	((scaled_value_bcd>>8)&0x0f);
		scaled_tens				= 	((scaled_value_bcd>>4)&0x0f);
		scaled_ones				= 	((scaled_value_bcd)&0x0f);







			
		number_display_foreground	=VGA_AQUA;
	//	number_display_background	=VGA_NAVY;





	

		
		if	(ten_thousands&0x04)
			{
			print_number(0x10);	//plus sign
			}
		else if
			(ten_thousands&0x08)
				{
				print_number(0x11);	//minus sign
				}
			else
				{
				print_blank_columns(char_pixel_width);
				}		
		
		


		print_blank_columns(blank_width);					
		if ((scaled_ten_thousands==0) && ((scaled_decimals & 0b0111) || !scaled_decimals))	//zero blanking done different, go back and change original
				{
				print_blank_columns(char_pixel_width);	//blank leading zeros
				put_UART(' ');
				}
		else
				{
				print_number(scaled_ten_thousands);
				put_UART(scaled_ten_thousands+'0');
				}

		if	(scaled_decimals&0b1000)
			{
			print_blank_columns(blank_width);			
			print_decimal_point();
			put_UART('.');
			}
				
		
		print_blank_columns(blank_width);									
		if ((scaled_ten_thousands==0) && (scaled_thousands==0) && ((scaled_decimals & 0b0011) || !scaled_decimals))
				{
				print_blank_columns(char_pixel_width);	//blank leading zeros
				put_UART(' ');
				}
		else
				{
				print_number(scaled_thousands);
				put_UART(scaled_thousands+'0');
				}				
				
		if	(scaled_decimals&0b0100)
			{
			print_blank_columns(blank_width);			
			print_decimal_point();
			put_UART('.');
			}



				
		print_blank_columns(blank_width);									
		if ((scaled_ten_thousands==0) && (scaled_thousands==0) && (scaled_hundreds==0) && ((scaled_decimals & 0b0001) || !scaled_decimals))
				{
				print_blank_columns(char_pixel_width);	//blank leading zeros
				put_UART(' ');
				}
		else
				{
				print_number(scaled_hundreds);
				put_UART(scaled_hundreds+'0');
				}					
				
				
				


		if	(scaled_decimals&0x02)
			{
			print_blank_columns(blank_width);			
			print_decimal_point();
			put_UART('.');
			}




		print_blank_columns(blank_width);									
		if ((scaled_ten_thousands==0) && (scaled_thousands==0) && (scaled_hundreds==0) && (scaled_tens==0) && ( !scaled_decimals))
				{
				print_blank_columns(char_pixel_width);	//blank leading zeros
				put_UART(' ');
				}
		else
				{
				print_number(scaled_tens);
				put_UART(scaled_tens+'0');
				}	





		
		if	(scaled_decimals&0x01)
			{
			print_blank_columns(blank_width);			
			print_decimal_point();
			put_UART('.');
			}


		print_blank_columns(blank_width);					
		print_number(scaled_ones);		
		put_UART(scaled_ones+'0');
		print_blank_columns(blank_width);			//hack to over write right side 
		print_blank_columns(blank_width);			//if not displaying decimal point
		

		
		
		
		
		
		
		
		
		put_UART(' ');		
		if (range==3)	//200 mV
			put_UART('m');	
		put_UART('A');	

		
		

		
	




	
		
		
		
		
		
		
		
		
		
		
		
		

		
		
		
		
		
		}
	
	else	//writing these blank characters take about 100ms
		{
		print_blank_columns(char_pixel_width);
		print_blank_columns(char_pixel_width);
		print_blank_columns(char_pixel_width);
		print_blank_columns(char_pixel_width);
		print_blank_columns(char_pixel_width);
		print_blank_columns(char_pixel_width);
	
		print_blank_columns(blank_width);			
		print_blank_columns(blank_width);			
		print_blank_columns(blank_width);			
		print_blank_columns(blank_width);			
		print_blank_columns(blank_width);			

		print_blank_columns(char_pixel_width);
		

		
		
		}

	

	number_display_x	=	display2_offset+75;

	ili9341_write_command(0x2a);	//column address set
	ili9341_write_data16(number_display_x);
	ili9341_write_data16(number_display_x+small_char_pixel_height);
	
	ili9341_write_command(0x2b);	//page address set
	ili9341_write_data16(180);
	ili9341_write_data16(319);	
	
	ili9341_write_command(0x2c);	//memory write

	if	(rel&&(  (function&0x06)==0x02))	//if in volts and rel show units of scaled value
			{
			if ((range)==0x03)
				print_small_characters(9);	//m
			else
				print_small_blank_columns(small_char_pixel_width);	//_
			print_small_characters(0);	//A
			
			
			
			number_display_foreground	=volts_colour;
			number_display_background	=background_colour;		
			
			
			
			
			}
		else
			{
			
							print_small_blank_columns(small_char_pixel_width);	//_

							print_small_blank_columns(small_char_pixel_width);	//_

			
			
			
			
			
			
			}
		



	
	
	

	put_string_UART("     ");


*/	
	
	
	
	
//	counter_bcd = bcd(counter_minutes);
//	put_UART(((counter_bcd>>16)&0x0f)+'0');
//	put_UART(((counter_bcd>>12)&0x0f)+'0');
//	put_UART(((counter_bcd>>8)&0x0f)+'0');
//	put_UART(((counter_bcd>>4)&0x0f)+'0');
//	put_UART(((counter_bcd)&0x0f)+'0');
//	put_string_UART("m ");







		counter_hundredth_seconds=counter_hundredth_seconds+41;
		if (counter_hundredth_seconds>=6000)
			{
			counter_hundredth_seconds 		=	counter_hundredth_seconds- 6000;
			counter_minutes_ones			=	counter_minutes_ones+1;
			if	(counter_minutes_ones>'9')
				{
				counter_minutes_ones 	= '0';
				counter_minutes_tens	=	counter_minutes_tens+1;
				if (counter_minutes_tens>'5')
					{
					counter_minutes_tens	=	'0';
					counter_hours_ones		=	counter_hours_ones+1;
					if (counter_hours_ones>'9')
						{
						counter_hours_ones='0';
						counter_hours_tens	=	counter_hours_tens+1;
						if (counter_hours_tens>'9')
							counter_hours_tens='0';
						}
					}
				}
			}


		
			
			
			
	put_UART(counter_hours_tens);
	put_UART(counter_hours_ones);
	put_string_UART("h ");

	put_UART(counter_minutes_tens);
	put_UART(counter_minutes_ones);
	put_string_UART("m ");


	counter_bcd = bcd(counter_hundredth_seconds);
//	put_UART(((counter_bcd>>16)&0x0f)+'0');
	put_UART(((counter_bcd>>12)&0x0f)+'0');
	put_UART(((counter_bcd>>8)&0x0f)+'0');
	put_UART('.');
	put_UART(((counter_bcd>>4)&0x0f)+'0');
	put_UART(((counter_bcd)&0x0f)+'0');
	put_string_UART("s");			
			
			
			
			
			
			
			
			
			


		
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
	if ((range==range_old)&&(function==function_old)&&(ten_thousands_old==ten_thousands)
		&&(thousands_old==thousands)&&(hundreds_old==hundreds)&&(tens_old==tens)&&(ones_old==ones))
			{
				put_string_UART("\r");
				//put_string_UART("\r\n");//testing


			}
		else
			{
				put_string_UART(" split \r\n");
				//counter=0;


		counter_hundredth_seconds=0;
		counter_hours_tens='0';
		counter_hours_ones='0';
		counter_minutes_tens='0';
		counter_minutes_ones='0';

	
			}

















	

	if ((range!=range_old)||(function!=function_old)||(	rel!=rel_old))
		{
		counter_hundredth_seconds=0;
		counter_hours_tens='0';
		counter_hours_ones='0';
		counter_minutes_tens='0';
		counter_minutes_ones='0';
		put_string_UART( "\x1b[2J");	//clear screen
		put_string_UART( "\x1b[1H");	//home
		}








		}

}


