#include  "setup_pic18f4420.h"
#include  "setup_ili9341.h"
#include  "fonts/fonts.h"

// using following line to compile for PIC18F4420
// xc8 --CHIP=18F4420 <files.c>

unsigned long  counter, counter_bcd;
unsigned int   number_display_x, number_display_y;
unsigned int   number_display_foreground, number_display_background;
unsigned int  ten_thousands, thousands, hundreds, tens, ones, display_bcd;
unsigned int  range, range_old, function, hv, db_scanning_mode;
enum u_fontsize {REGULAR, SMALL, RANGE} fontsize; 

void print_character(unsigned int digit, enum u_fontsize fontsize) {
  unsigned int row = 0;                //  0 -> (character_width-1)
  unsigned int column_pixel = 0;       //  0 -> (character_height-1)
  unsigned int column_byte_bit = 0;    //  0 -> 7 which pixel in column_byte_index
  unsigned int work_column_byte = 0;   //  copy of column_byte_index 
  
  unsigned int char_width, char_height, bytes_per_row; 
  if (fontsize == REGULAR) {
    char_width = FONT_REGULAR_CHARACTER_WIDTH;
    char_height = FONT_REGULAR_CHARACTER_HEIGHT;
    bytes_per_row = FONT_REGULAR_BYTES_PER_ROW;
  } else if (fontsize == SMALL) {
    char_width = FONT_SMALL_CHARACTER_WIDTH;
    char_height = FONT_SMALL_CHARACTER_HEIGHT;
    bytes_per_row = FONT_SMALL_BYTES_PER_ROW;
  } else if (fontsize == RANGE) {
    char_width = FONT_RANGE_CHARACTER_WIDTH;
    char_height = FONT_RANGE_CHARACTER_HEIGHT;
    bytes_per_row = FONT_RANGE_BYTES_PER_ROW;
  }
  unsigned int lookup  = digit * char_width * bytes_per_row; 
  while (row < char_width) {
    column_byte_bit = 0;
    column_pixel    = 0;
    while (column_pixel < char_height) {
      work_column_byte  = fontBitmaps[lookup++];
      column_byte_bit = 0;
      while ((column_byte_bit < 8) && (column_pixel < (char_height + 1))) {
        if (work_column_byte & 0x80) {
            display_write_data16(number_display_foreground);
        } else {
            display_write_data16(number_display_background);
        }
        work_column_byte = work_column_byte << 1;
        column_byte_bit  = column_byte_bit + 1;
        column_pixel     = column_pixel + 1;
      }
    }
  row++;
  } 
}

void print_blank_columns(unsigned int blank_columns, enum u_fontsize fontsize) {
  unsigned int char_height;
  if(fontsize == REGULAR){
    char_height = FONT_REGULAR_CHARACTER_HEIGHT;
  } else if (fontsize == SMALL) {
    char_height = FONT_SMALL_CHARACTER_HEIGHT;
  } else if(fontsize  == RANGE) {
    char_height = FONT_RANGE_CHARACTER_HEIGHT;
  }
  unsigned int work_counter = blank_columns * (char_height + 1);
  while(work_counter>0) {
    display_write_data16(number_display_background);
    work_counter--;    
  }
}

void print_decimal_point()
  {
  unsigned int  row=0;          //  0 -> (character_width-1)
  unsigned int  column_pixel=0;     //  0 -> (character_height-1)
  unsigned int  column_byte_bit=0;    //  0 -> 7 which pixel in column_byte_index
  unsigned int  work_column_byte;   //  copy of column_byte_index (need to play with to get past unused bits in font table)

  unsigned int  lookup  = 0;      //  how for into font table our byte is

    while (row < FONT_REGULAR_DECIMAL_POINT_WIDTH){
  
      column_byte_bit =   0;
      column_pixel  = 0;

      while (column_pixel<FONT_REGULAR_CHARACTER_HEIGHT)
        {
          work_column_byte  = fontDecimalPoint[lookup++];
          column_byte_bit = 0;

          while ((column_byte_bit<8)&&(column_pixel<(FONT_REGULAR_CHARACTER_HEIGHT+1)))
          {
          if (work_column_byte & 0x80)
              display_write_data16(number_display_foreground);
            else
              display_write_data16(number_display_background);

          work_column_byte = work_column_byte<<1;
          column_byte_bit =   column_byte_bit +1;
          column_pixel  = column_pixel+1;
          }
    
        }
      row++;
      } 
  }
  


void main(void){
  mc_pin_setup();
  
  number_display_foreground = foreground_colour;
  number_display_background = background_colour; 
  
  display_setup();
  
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

  display_write_command(0x2a);  //column address set
  display_write_data16(number_display_x);
  display_write_data16(number_display_x+FONT_REGULAR_CHARACTER_HEIGHT);
  
  display_write_command(0x2b);  //page address set
  display_write_data16(5);
  display_write_data16(319);  
  
  display_write_command(0x2c);  //memory write

  
  

  if  (ten_thousands&0x04)
    {
    print_character(0x10, REGULAR); //plus sign
    }
  else if
    (ten_thousands&0x08)
      {
      print_character(0x11, REGULAR); //minus sign
      }
    else
      print_blank_columns(FONT_REGULAR_CHARACTER_WIDTH, REGULAR);


  print_blank_columns(FONT_REGULAR_BLANK_WIDTH, REGULAR);
  if (((ten_thousands&0x01)==0) &&  ((hundreds&0x10)|(tens&0x10)|(ones&0x10)|db_scanning_mode))
    print_blank_columns(FONT_REGULAR_CHARACTER_WIDTH, REGULAR);  //blank leading zeros
  else
    print_character(ten_thousands&0x01, REGULAR); 


  print_blank_columns(FONT_REGULAR_BLANK_WIDTH, REGULAR);
  if  (thousands&0x10)
    {
    print_decimal_point();
    print_blank_columns(FONT_REGULAR_BLANK_WIDTH, REGULAR);
    }
    
    
  if ((ten_thousands&0x01)==0&&(thousands&0x0f)==0&&((tens&0x10)|(ones&0x10)))  
    print_blank_columns(FONT_REGULAR_CHARACTER_WIDTH, REGULAR);  //blank leading zeros
  else
    print_character(thousands&0x0f, REGULAR);

  print_blank_columns(FONT_REGULAR_BLANK_WIDTH, REGULAR);
  if  (hundreds&0x10)
    {
    print_decimal_point();
    print_blank_columns(FONT_REGULAR_BLANK_WIDTH, REGULAR);
    }
    
  if ((ten_thousands&0x01)==0&&(thousands&0x0f)==0&&(hundreds&0x0f)==0&&(ones&0x10))  
    print_blank_columns(FONT_REGULAR_CHARACTER_WIDTH, REGULAR);  //blank leading zeros
  else  
    print_character(hundreds&0x0f, REGULAR);
    
  print_blank_columns(FONT_REGULAR_BLANK_WIDTH, REGULAR);
  if  (tens&0x10)
    {
    print_decimal_point();
    print_blank_columns(FONT_REGULAR_BLANK_WIDTH, REGULAR);
    }
  print_character(tens&0x0f, REGULAR);


  if  ((ones<0x19)||(tens<0x19))  //hack to not overwrite right side of display during error
  {
  print_blank_columns(FONT_REGULAR_BLANK_WIDTH, REGULAR);
  if  (ones&0x10)
    {
    print_decimal_point();
    print_blank_columns(FONT_REGULAR_BLANK_WIDTH, REGULAR);
    }
  print_character(ones&0x0f, REGULAR);
  }

  if  (db_scanning_mode)
    print_blank_columns(FONT_REGULAR_CHARACTER_WIDTH, REGULAR);  //hack to clear right side when no decimal point
  
  
  number_display_x  = 95;

  display_write_command(0x2a);  //column address set
  display_write_data16(number_display_x);
  display_write_data16(number_display_x + FONT_SMALL_CHARACTER_HEIGHT);
  
  display_write_command(0x2b);  //page address set
  display_write_data16(180);
  display_write_data16(319);  
  
  display_write_command(0x2c);  //memory write

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
      print_character(9, SMALL);  //m
    else
      print_blank_columns(FONT_SMALL_CHARACTER_WIDTH, SMALL);  //_

    print_character(6, SMALL);  //V
    
    if (function&0x01)
      {
      number_display_foreground = ac_colour;
      print_character(0, SMALL);  //A
      }
    else
      print_character(3, SMALL);  //D

    print_character(2, SMALL);  //C
    number_display_foreground = volts_colour;
  }


  if ((function&0x06)==0x04)  // if function = amps
  {
    number_display_foreground = amps_colour;
  
    if ((range)==0x03)
      print_character(12, SMALL); //u
    else
      print_character(9, SMALL);  //m

    print_character(0, SMALL);  //a

    if (function&0x01)
      {
      number_display_foreground = ac_colour;
      print_character(0, SMALL);  //A
      }
    else
      print_character(3, SMALL);  //D

    print_character(2, SMALL);  //C
    number_display_foreground = amps_colour;
  }

  
  if (((function&0x06)==0x00)     && (range!=1)&&(range!=2))  // if function = ohms
  {
    number_display_foreground = ohms_colour;
    print_blank_columns(FONT_SMALL_CHARACTER_WIDTH, SMALL);  //_
    print_blank_columns(FONT_SMALL_CHARACTER_WIDTH, SMALL);  //_ 
    if ((range)==0x03)
      print_blank_columns(FONT_SMALL_CHARACTER_WIDTH, SMALL);  //_
    else  if  ((range)==0x00)
        print_character(4, SMALL);  //M
      else
        print_character(8, SMALL);  //k
    print_character(11, SMALL); //omega
  } 
  

  if (((function&0x06)==0x00)     && ((range==1)||(range==2)))  // if function = Siemens
  {
    number_display_foreground = siemens_colour;
    print_blank_columns(FONT_SMALL_CHARACTER_WIDTH, SMALL);  //_
    print_blank_columns(FONT_SMALL_CHARACTER_WIDTH, SMALL);  //_ 
    if ((range)==0x02)
        print_character(9, SMALL);  //m
    else
        print_character(10, SMALL); //n
    print_character(5, SMALL);  //S
  }   

  if ((function&0x06)==0x06)  // if function = dB 
  {
  
    if  (db_scanning_mode)
      {
      number_display_foreground = db_colour;
      print_blank_columns(FONT_SMALL_CHARACTER_WIDTH, SMALL);  //_
      print_blank_columns(FONT_SMALL_CHARACTER_WIDTH, SMALL);  //_ 
      print_blank_columns(FONT_SMALL_CHARACTER_WIDTH, SMALL);  //_ 
      print_character(11, SMALL); //omega
      }

    else
      {
      number_display_foreground = db_colour;
      print_blank_columns(FONT_SMALL_CHARACTER_WIDTH, SMALL);  //_
      print_blank_columns(FONT_SMALL_CHARACTER_WIDTH, SMALL);  //_ 
      print_character(7, SMALL);  //d
      print_character(1, SMALL);  //B
      }
  }   
  
  
  number_display_x  = 95;

  display_write_command(0x2a);  //column address set
  display_write_data16(number_display_x);
  display_write_data16(number_display_x + FONT_RANGE_CHARACTER_HEIGHT);
  
  display_write_command(0x2b);  //page address set
  display_write_data16(100);
  display_write_data16(319);  
  
  display_write_command(0x2c);  //memory write


  // if function = dB don't show range, should show
  if ((function&0x06)==0x06) {  
    print_character(0, RANGE);//_
    print_character(0, RANGE);//_
    print_character(0, RANGE);//_
    print_character(0, RANGE);//_
  } else { 
    if (range==0) { //20 Mohm
      if  ((function&0b0110)==0) {
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
        print_character(2, RANGE);//2
        print_character(1, RANGE);//0
      }
      else {
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
      }
    }
    if (range==1) { //200 nS
      if  ((function&0b0110)==0) {
        print_character(0, RANGE);//_
        print_character(2, RANGE);//2
        print_character(1, RANGE);//0
        print_character(1, RANGE);//0
      } else {
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
      }
    }
      
    if (range==2) { //2 mS
      if  ((function&0b0110)==0) {
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
        print_character(2, RANGE);//2
      } else {
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
        print_character(0, RANGE);//_
      }
    }
    if (range==3) { //200 ohm,uA,mV
      print_character(0, RANGE);//_
      print_character(2, RANGE);//2
      print_character(1, RANGE);//0
      print_character(1, RANGE);//0
    }

    if (range==4) { //200 kohm,mA,V
      print_character(0, RANGE);//_
      print_character(2, RANGE);//2
      print_character(1, RANGE);//0
      print_character(1, RANGE);//0
    }

    if (range==5) { //20 kohm,mA,V
      print_character(0, RANGE);//_
      print_character(0, RANGE);//_
      print_character(2, RANGE);//2
      print_character(1, RANGE);//0
    }

    if (range==6) { //2 kohm,mA,V
      print_character(0, RANGE);//_
      print_character(0, RANGE);//_
      print_character(0, RANGE);//_
      print_character(2, RANGE);//2
    }

    if (range==7) { //2000 kohm,mA,V
      print_character(2, RANGE);//2
      print_character(1, RANGE);//0
      print_character(1, RANGE);//0
      print_character(1, RANGE);//0
    }

  } 


  number_display_x  = 70;

  display_write_command(0x2a);  //column address set
  display_write_data16(number_display_x);
  display_write_data16(number_display_x + FONT_RANGE_CHARACTER_HEIGHT);
  
  display_write_command(0x2b);  //page address set
  display_write_data16(272);
  display_write_data16(319);  
  
  display_write_command(0x2c);  //memory write

  //if (function&0x08)  // /REL <button>
  if (ten_thousands&0x10) // REL <display>
      {
      print_character(6, RANGE);//R
      print_character(3, RANGE);//E
      print_character(5, RANGE);//L
      }
    else
      {
      print_character(0, RANGE);//_
      print_character(0, RANGE);//_
      print_character(0, RANGE);//_
      }
        
      
  number_display_x  = 45;
      
  display_write_command(0x2a);  //column address set
  display_write_data16(number_display_x);
  display_write_data16(number_display_x + FONT_RANGE_CHARACTER_HEIGHT);
  
  display_write_command(0x2b);  //page address set
  display_write_data16(272);
  display_write_data16(319);  
  
  display_write_command(0x2c);  //memory write      

  if (hv) // HV  ***do lighting bolt?
      {
      print_character(4, RANGE);//H
      print_character(7, RANGE);//V
      }
    else  
      {
      print_character(0, RANGE);//_
      print_character(0, RANGE);//_
      }

  counter++;

  }

}

