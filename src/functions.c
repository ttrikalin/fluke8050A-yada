#include "functions.h"

unsigned long bcd (unsigned int val)
{
  unsigned long bcdresult = 0; char i;

  for (i = 0; val; i++)
  {
    ((char*)&bcdresult)[i / 2] |= i & 1 ? (val % 10) << 4 : (val % 10) & 0xf;
    val /= 10;
  }
  return bcdresult;

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


