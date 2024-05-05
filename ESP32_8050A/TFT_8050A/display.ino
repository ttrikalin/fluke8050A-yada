#include "TFT_8050A.h"

void display_monitor_initialize(void) {
  display_monitor.state = DISPLAY_MONITOR_STATE_INIT;

  display_monitor.active_background_color = TFT_BLACK;
  display_monitor.active_text_color = TFT_WHITE;

  display_monitor.relative_reference = 0.0;
  display_monitor.digits_str = "-.0000";
  display_monitor.impedance_reference_str = fluke8050a_HARDWARE_IMPEDANCE_REFERENCE_STR;

  display_monitor.non_high_voltage_theme.splash_background_color      = TFT_DARKGREEN;
  display_monitor.non_high_voltage_theme.splash_text_color            = TFT_WHITE;
  display_monitor.non_high_voltage_theme.voltage_background_color     = TFT_BLACK;
  display_monitor.non_high_voltage_theme.voltage_text_color           = TFT_WHITE;
  display_monitor.non_high_voltage_theme.current_background_color     = TFT_BLACK;
  display_monitor.non_high_voltage_theme.current_text_color           = TFT_WHITE;
  display_monitor.non_high_voltage_theme.resistance_background_color  = TFT_BLACK;
  display_monitor.non_high_voltage_theme.resistance_text_color        = TFT_WHITE;
  display_monitor.non_high_voltage_theme.conductance_background_color = TFT_BLACK;
  display_monitor.non_high_voltage_theme.conductance_text_color       = TFT_WHITE;
  display_monitor.non_high_voltage_theme.gain_background_color        = TFT_BLACK;
  display_monitor.non_high_voltage_theme.gain_text_color              = TFT_WHITE;
  display_monitor.non_high_voltage_theme.invalid_background_color     = TFT_RED;
  display_monitor.non_high_voltage_theme.invalid_text_color           = TFT_WHITE;

  display_monitor.high_voltage_theme.splash_background_color      = TFT_GREEN;
  display_monitor.high_voltage_theme.splash_text_color            = TFT_WHITE;
  display_monitor.high_voltage_theme.voltage_background_color     = TFT_BLACK;
  display_monitor.high_voltage_theme.voltage_text_color           = TFT_RED;
  display_monitor.high_voltage_theme.current_background_color     = TFT_BLACK;
  display_monitor.high_voltage_theme.current_text_color           = TFT_RED;
  display_monitor.high_voltage_theme.resistance_background_color  = TFT_BLACK;
  display_monitor.high_voltage_theme.resistance_text_color        = TFT_RED;
  display_monitor.high_voltage_theme.conductance_background_color = TFT_BLACK;
  display_monitor.high_voltage_theme.conductance_text_color       = TFT_RED;
  display_monitor.high_voltage_theme.gain_background_color        = TFT_BLACK;
  display_monitor.high_voltage_theme.gain_text_color              = TFT_RED;
  display_monitor.high_voltage_theme.invalid_background_color     = TFT_RED;
  display_monitor.high_voltage_theme.invalid_text_color           = TFT_WHITE;

  tft.init();
  tft.setRotation(TFT_SCREEN_ROTATION);
  tft.fillScreen(display_monitor.active_background_color);
}


void display_monitor_tasks(void) {
  switch(display_monitor.state) {  

    case DISPLAY_MONITOR_STATE_INIT: 
      display_monitor.state = DISPLAY_MONITOR_STATE_WAIT;
      show_splash_screen(); 
      break;

    case DISPLAY_MONITOR_STATE_WAIT:
      if (!digits_monitor.in_strobe_phase) {
        display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_HIGH_VOLTAGE;
      }
      break;

    case DISPLAY_MONITOR_STATE_SHOW_HIGH_VOLTAGE:
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_VALUE;
      show_high_voltage();         
      break;

    case DISPLAY_MONITOR_STATE_SHOW_VALUE:
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_REFERENCE_VALUE;
      show_digits(); 
      break; 

    case DISPLAY_MONITOR_STATE_SHOW_REFERENCE_VALUE: 
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_UNIT;
      show_reference_value(); 
      break;

    case DISPLAY_MONITOR_STATE_SHOW_UNIT:
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_LOW_BATTERY;
      show_unit(); 
      break;

    case DISPLAY_MONITOR_STATE_SHOW_LOW_BATTERY: 
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_DIODE;
      if(contents_monitor.battery != NO_BATTERY){
        show_battery();   
      }
      break;

    case DISPLAY_MONITOR_STATE_SHOW_DIODE:
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_IMPEDANCES;
      if (contents_monitor.diode_style != NO_DIODE) {
        show_diode(); 
      }
      show_diode(); 
      break;

    case DISPLAY_MONITOR_STATE_SHOW_IMPEDANCES:
      display_monitor.state = DISPLAY_MONITOR_STATE_WAIT;
      show_impedances(); 
      break;

    default:
      break; 
  }
}


void use_colors(unsigned int background_color, 
                unsigned int text_color) {
  if(display_monitor.active_background_color != background_color) {
    display_monitor.active_background_color = background_color; 
    tft.fillScreen(background_color);
  }
  if(display_monitor.active_text_color != text_color) {
    display_monitor.active_text_color = text_color; 
    tft.setTextColor(display_monitor.active_text_color, 
                     display_monitor.active_background_color, 
                     true);  
  }
}


void update_colors(void) {
  color_themes * theme; 
  if(contents_monitor.high_voltage) {
    theme = &display_monitor.high_voltage_theme;
  } else {
    theme = &display_monitor.non_high_voltage_theme;
  } 
  
  if(contents_monitor.quantity == NO_QUANTITY) 
  {
    use_colors(theme->splash_background_color, 
                 theme->splash_text_color);
  } 
  else if (contents_monitor.quantity == RESISTANCE)
  {
    use_colors(theme->resistance_background_color, 
                 theme->resistance_text_color);
  }
  else if (contents_monitor.quantity == VOLTAGE)
  {
    use_colors(theme->voltage_background_color, 
                 theme->voltage_text_color);
  }
  else if (contents_monitor.quantity == CURRENT)
  {
    use_colors(theme->current_background_color, 
                 theme->current_text_color);
  } 
  else if (contents_monitor.quantity == CONDUCTANCE)
  {
    use_colors(theme->conductance_background_color, 
                 theme->conductance_text_color);
  }
  else if (contents_monitor.quantity == GAIN)
  {
    use_colors(theme->gain_background_color, 
                 theme->gain_text_color);
  }
}

void show_splash_screen(void) {
  update_colors();
  unsigned char x = (TFT_WIDTH - W_SPLASH_FLUKE)>>1; 
  unsigned char y = (TFT_HEIGHT - H_SPLASH_FLUKE)>>2;


  // First Splash Screen 

  tft.drawBitmap(x, y, splashFluke, W_SPLASH_FLUKE, H_SPLASH_FLUKE, display_monitor.active_text_color, display_monitor.active_background_color);
  
  tft.loadFont(AA_FONT_MEDIUM);

  tft.setCursor(
    (TFT_WIDTH - tft.textWidth(firmware_information.hardware))>>1, 
    y += H_SPLASH_FLUKE + tft.fontHeight() +3
  );
  tft.println(firmware_information.hardware);
  

  delay(TFT_SPLASH_SCREEN_DURATION);   
  tft.fillScreen(display_monitor.active_background_color);


  // Second Splash Screen 
  y = ((TFT_HEIGHT - H_SPLASH_FLUKE)>>2) + 10;
  tft.setCursor(
    (TFT_WIDTH - tft.textWidth(firmware_information.author))>>1, 
    y
  );
  tft.println(firmware_information.author);
  y += tft.fontHeight() + 10; 

  tft.unloadFont();
  tft.loadFont(AA_FONT_SMALL);
  
  tft.setCursor(
    (TFT_WIDTH - tft.textWidth(firmware_information.version))>>1, 
    y
  );
  tft.println(firmware_information.version);


  tft.setCursor(
    (TFT_WIDTH - tft.textWidth(firmware_information.github1))>>1, 
    y += tft.fontHeight() + 5 
  );
  tft.println(firmware_information.github1);
  tft.setCursor(
    (TFT_WIDTH - tft.textWidth(firmware_information.github2))>>1, 
    y += tft.fontHeight() 
  );
  tft.println(firmware_information.github2);


  tft.unloadFont();
  delay(TFT_SPLASH_SCREEN_DURATION<<1);   
  tft.fillScreen(display_monitor.active_background_color);
} 


void draw_large_sign(signs sign, unsigned char &x, unsigned char &y) {
  if(sign != NO_SIGN) {
    img.drawBitmap(
      x, 
      y + OFFSET_SIGN_LG, 
      symbolSign_lg[contents_monitor.sign], 
      W_SIGN_LG, 
      H_SIGN_LG, 
      display_monitor.active_text_color, 
      TFT_TRANSPARENT
      );  
  }
  x += W_SIGN_LG;
}


void draw_large_decimal_point(unsigned char &x, unsigned char &y){
  img.drawBitmap(
    x, 
    y, 
    &dp_lg[0], 
    W_DP_LG, 
    H_DP_LG, 
    display_monitor.active_text_color, 
    TFT_TRANSPARENT
  );
  x += W_DP_LG;
}

void draw_large_digit(unsigned char d, unsigned char &x, unsigned char &y){
  img.drawBitmap(
    x, 
    y, 
    digit_lg[d], 
    W_DIGIT_LG, 
    H_DIGIT_LG, 
    display_monitor.active_text_color, 
    TFT_TRANSPARENT
  );
  x += W_DIGIT_LG;
}


void draw_low_battery(unsigned char &x, unsigned char &y){
  tft.drawBitmap(
    x, 
    y, 
    &symbolLoBatt[0], 
    W_LOBATT, 
    H_LOBATT, 
    TFT_RED, //display_monitor.active_text_color, 
    display_monitor.active_background_color
  );
  x += W_LOBATT;
}

void format_large_digits(void){
  img.setColorDepth(8);
  img.createSprite(W_IMG_MAIN, H_IMG_MAIN);

  img.fillSprite(TFT_TRANSPARENT);

  unsigned char x = 0;
  unsigned char y = 0;

  draw_large_sign(contents_monitor.sign, x, y);
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_ZERO){
    draw_large_decimal_point(x, y);
  } 
  draw_large_digit(digits_monitor.st0_value0, x, y);
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_ONE){
    draw_large_decimal_point(x, y);
  } 
  draw_large_digit(digits_monitor.st1_value+1, x, y);
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_TWO){
    draw_large_decimal_point(x, y);
  } 
  draw_large_digit(digits_monitor.st2_value+2, x, y);
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_THREE){
    draw_large_decimal_point(x, y);
  } 
  draw_large_digit(digits_monitor.st3_value+3, x, y);


  img.pushSprite(X_DIGITS, Y_DIGITS, TFT_TRANSPARENT);
  img.deleteSprite();
}




void show_high_voltage(void){
  update_colors();
  tft.setCursor(X_HIGH_VOLTAGE, Y_HIGH_VOLTAGE);
  tft.setTextPadding(tft.textWidth("HV!"));
  if(contents_monitor.high_voltage) {
    tft.println("HV!");
  } else {
    tft.fillRect(X_HIGH_VOLTAGE, Y_HIGH_VOLTAGE, 25, 15, display_monitor.active_background_color);
  }
}

void show_digits(void){
  update_colors();
  tft.fillRect(X_DIGITS, Y_DIGITS, W_IMG_MAIN, H_IMG_MAIN, display_monitor.active_background_color);
  format_large_digits();
}
void show_reference_value(void){
  update_colors();
  tft.loadFont(AA_FONT_SMALL);
  tft.setCursor(X_REL, Y_REL);
  tft.println("9999");
} 

void show_unit(void) {
  update_colors();
  tft.loadFont(AA_FONT_MEDIUM);
  tft.setCursor(X_UNIT, Y_UNIT);
  tft.println("U");
}


void show_battery(void) {
  if (contents_monitor.battery == LOW_BATTERY) 
  {
    unsigned char y = Y_BATTERY;
    unsigned char x = X_BATTERY;
    draw_low_battery(x, y);
  } 
  else 
  {
    tft.fillRect(X_BATTERY, Y_BATTERY, W_LOBATT, H_LOBATT, display_monitor.active_background_color);
  }
} 

void show_diode(void){
  update_colors();
  tft.loadFont(AA_FONT_SMALL);
  tft.setCursor(X_DIODE, Y_DIODE);
  tft.println("DIODE");
}

void show_impedances(void){

}


