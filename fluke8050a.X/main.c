#include "setup_pic18f4420.h"
#include "setup_ili9341.h"
#include "fonts/fonts.h"
#include "config_color_scheme.h"

// using following line to compile for PIC18F4420
// xc8 --CHIP=18F4420 <files.c>

extern unsigned int  foreground_color, background_color;
//unsigned long  counter;
unsigned int  number_display_x, number_display_y;
unsigned int  ten_thousands, thousands, hundreds, tens, ones, display_bcd;
unsigned int  range, range_old, function, hv, db_scanning_mode;



void main(void){
  foreground_color = CONFIG_COLOR_FOREGROUND;
  background_color = CONFIG_COLOR_BACKGROUND; 
  
  mc_pin_setup();
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
      if (range_old!=range) //three sets of strobe pulse when first going in REF Z- disregard first two strobe sets
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


//    if (counter>20000) {
//        counter=0;
//    }


    
    /*********************************************************************************/
    // 20 and (5, 319) -- DISPLAYED_VALUE
    /*********************************************************************************/

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
      print_regular_character(0x10); //plus sign
      }
    else if
      (ten_thousands&0x08)
        {
        print_regular_character(0x11); //minus sign
        }
      else
        print_regular_blank_columns(FONT_REGULAR_CHARACTER_WIDTH);


    print_regular_blank_columns(FONT_REGULAR_BLANK_WIDTH);
    if (((ten_thousands&0x01)==0) &&  ((hundreds&0x10)|(tens&0x10)|(ones&0x10)|db_scanning_mode))
      print_regular_blank_columns(FONT_REGULAR_CHARACTER_WIDTH);  //blank leading zeros
    else
      print_regular_character(ten_thousands&0x01); 


    print_regular_blank_columns(FONT_REGULAR_BLANK_WIDTH);
    if  (thousands&0x10)
      {
      print_decimal_point();
      print_regular_blank_columns(FONT_REGULAR_BLANK_WIDTH);
      }
      
      
    if ((ten_thousands&0x01)==0&&(thousands&0x0f)==0&&((tens&0x10)|(ones&0x10)))  
      print_regular_blank_columns(FONT_REGULAR_CHARACTER_WIDTH);  //blank leading zeros
    else
      print_regular_character(thousands&0x0f);

    print_regular_blank_columns(FONT_REGULAR_BLANK_WIDTH);
    if  (hundreds&0x10)
      {
      print_decimal_point();
      print_regular_blank_columns(FONT_REGULAR_BLANK_WIDTH);
      }
      
    if ((ten_thousands&0x01)==0&&(thousands&0x0f)==0&&(hundreds&0x0f)==0&&(ones&0x10))  
      print_regular_blank_columns(FONT_REGULAR_CHARACTER_WIDTH);  //blank leading zeros
    else  
      print_regular_character(hundreds&0x0f);
      
    print_regular_blank_columns(FONT_REGULAR_BLANK_WIDTH);
    if  (tens&0x10)
      {
      print_decimal_point();
      print_regular_blank_columns(FONT_REGULAR_BLANK_WIDTH);
      }
    print_regular_character(tens&0x0f);


    if  ((ones<0x19)||(tens<0x19))  //hack to not overwrite right side of display during error
    {
    print_regular_blank_columns(FONT_REGULAR_BLANK_WIDTH);
    if  (ones&0x10)
      {
      print_decimal_point();
      print_regular_blank_columns(FONT_REGULAR_BLANK_WIDTH);
      }
    print_regular_character(ones&0x0f);
    }

    if  (db_scanning_mode)
      print_regular_blank_columns(FONT_REGULAR_CHARACTER_WIDTH);  //hack to clear right side when no decimal point
    
    
    /*********************************************************************************/
    // 95 and (180, 319) -- FUNCTION
    /*********************************************************************************/

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
      foreground_color = volts_colour;
      if ((range)==0x03)
        print_small_character(9);  //m
      else
        print_small_blank_columns(FONT_SMALL_CHARACTER_WIDTH);  //_

      print_small_character(6);  //V
      
      if (function&0x01)
        {
        foreground_color = ac_colour;
        print_small_character(0);  //A
        }
      else
        print_small_character(3);  //D

      print_small_character(2);  //C
      foreground_color = volts_colour;
    }


    if ((function&0x06)==0x04)  // if function = amps
    {
      foreground_color = amps_colour;
    
      if ((range)==0x03)
        print_small_character(12); //u
      else
        print_small_character(9);  //m

      print_small_character(0);  //A

      if (function&0x01)
        {
        foreground_color = ac_colour;
        print_small_character(0);  //A
        }
      else
        print_small_character(3);  //D

      print_small_character(2);  //C
      foreground_color = amps_colour;
    }

    
    if (((function&0x06)==0x00)     && (range!=1)&&(range!=2))  // if function = ohms
    {
      foreground_color = ohms_colour;
      print_small_blank_columns(FONT_SMALL_CHARACTER_WIDTH);  //_
      print_small_blank_columns(FONT_SMALL_CHARACTER_WIDTH);  //_ 
      if ((range)==0x03)
        print_small_blank_columns(FONT_SMALL_CHARACTER_WIDTH);  //_
      else  if  ((range)==0x00)
          print_small_character(4);  //M
        else
          print_small_character(8);  //k
      print_small_character(11); //omega
    } 
    

    if (((function&0x06)==0x00)     && ((range==1)||(range==2)))  // if function = Siemens
    {
      foreground_color = siemens_colour;
      print_small_blank_columns(FONT_SMALL_CHARACTER_WIDTH);  //_
      print_small_blank_columns(FONT_SMALL_CHARACTER_WIDTH);  //_ 
      if ((range)==0x02)
          print_small_character(9);  //m
      else
          print_small_character(10); //n
      print_small_character(5);  //S
    }   

    if ((function&0x06)==0x06)  // if function = dB 
    {
    
      if  (db_scanning_mode)
        {
        foreground_color = db_colour;
        print_small_blank_columns(FONT_SMALL_CHARACTER_WIDTH);  //_
        print_small_blank_columns(FONT_SMALL_CHARACTER_WIDTH);  //_ 
        print_small_blank_columns(FONT_SMALL_CHARACTER_WIDTH);  //_ 
        print_small_character(11); //omega
        }

      else
        {
        foreground_color = db_colour;
        print_small_blank_columns(FONT_SMALL_CHARACTER_WIDTH);  //_
        print_small_blank_columns(FONT_SMALL_CHARACTER_WIDTH);  //_ 
        print_small_character(7);  //d
        print_small_character(1);  //B
        }
    }   
    
    /*********************************************************************************/
    // 95 and (100, 319) -- RANGE
    /*********************************************************************************/
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
      print_range_character(0);//_
      print_range_character(0);//_
      print_range_character(0);//_
      print_range_character(0);//_
    } else { 
      if (range==0) { //20 Mohm
        if  ((function&0b0110)==0) {
          print_range_character(0);//_
          print_range_character(0);//_
          print_range_character(2);//2
          print_range_character(1);//0
        }
        else {
          print_range_character(0);//_
          print_range_character(0);//_
          print_range_character(0);//_
          print_range_character(0);//_
        }
      }
      if (range==1) { //200 nS
        if  ((function&0b0110)==0) {
          print_range_character(0);//_
          print_range_character(2);//2
          print_range_character(1);//0
          print_range_character(1);//0
        } else {
          print_range_character(0);//_
          print_range_character(0);//_
          print_range_character(0);//_
          print_range_character(0);//_
        }
      }
        
      if (range==2) { //2 mS
        if  ((function&0b0110)==0) {
          print_range_character(0);//_
          print_range_character(0);//_
          print_range_character(0);//_
          print_range_character(2);//2
        } else {
          print_range_character(0);//_
          print_range_character(0);//_
          print_range_character(0);//_
          print_range_character(0);//_
        }
      }
      if (range==3) { //200 ohm,uA,mV
        print_range_character(0);//_
        print_range_character(2);//2
        print_range_character(1);//0
        print_range_character(1);//0
      }

      if (range==4) { //200 kohm,mA,V
        print_range_character(0);//_
        print_range_character(2);//2
        print_range_character(1);//0
        print_range_character(1);//0
      }

      if (range==5) { //20 kohm,mA,V
        print_range_character(0);//_
        print_range_character(0);//_
        print_range_character(2);//2
        print_range_character(1);//0
      }

      if (range==6) { //2 kohm,mA,V
        print_range_character(0);//_
        print_range_character(0);//_
        print_range_character(0);//_
        print_range_character(2);//2
      }

      if (range==7) { //2000 kohm,mA,V
        print_range_character(2);//2
        print_range_character(1);//0
        print_range_character(1);//0
        print_range_character(1);//0
      }

    } 


    /*********************************************************************************/
    // 70 and (272, 319) -- REL
    /*********************************************************************************/
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
        print_range_character(6);//R
        print_range_character(3);//E
        print_range_character(5);//L
        }
      else
        {
        print_range_character(0);//_
        print_range_character(0);//_
        print_range_character(0);//_
        }
          
    /*********************************************************************************/
    // 45 and (272, 319)  -- HV 
    /*********************************************************************************/    
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
        print_range_character(4);//H
        print_range_character(7);//V
        }
      else  
        {
        print_range_character(0);//_
        print_range_character(0);//_
        }

    //counter++;

  }

}


