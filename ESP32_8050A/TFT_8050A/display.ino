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

  large_decimal_point.width = W_DP_LG; 
  large_decimal_point.height = H_DP_LG; 
  large_decimal_point.y_offset = 0; 
  large_decimal_point.symbol = dp_lg; 

  small_decimal_point.width = W_DP_SM; 
  small_decimal_point.height = H_DP_SM; 
  small_decimal_point.y_offset = 0; 
  small_decimal_point.symbol = dp_sm; 

  large_digit.width = W_DIGIT_LG;
  large_digit.height = H_DIGIT_LG;
  large_digit.y_offset = 0; 
  large_digit.symbols = digit_lg; 

  small_digit.width = W_DIGIT_SM;
  small_digit.height = H_DIGIT_SM;
  small_digit.y_offset = 0; 
  small_digit.symbols = digit_sm; 

  large_sign.width = W_SIGN_LG; 
  large_sign.height = H_SIGN_LG; 
  large_sign.y_offset = 0; 
  large_sign.symbols = sign_lg; 

  small_sign.width = W_SIGN_SM; 
  small_sign.height = H_SIGN_SM; 
  small_sign.y_offset = Y_OFFSET_SIGN_SM; 
  small_sign.symbols = sign_sm; 

  large_unit_symbol.width = W_UNIT_LG;
  large_unit_symbol.height = H_UNIT_LG;
  large_unit_symbol.y_offset = 0;
  large_unit_symbol.symbols = unit_lg;

  small_unit_symbol.width = W_UNIT_SM;
  small_unit_symbol.height = H_UNIT_SM;
  small_unit_symbol.y_offset = 0;
  small_unit_symbol.symbols = unit_sm;

  large_mode_symbol.width = W_MODE_LG;
  large_mode_symbol.height = H_MODE_LG;
  large_mode_symbol.y_offset = 0;
  large_mode_symbol.symbols = mode_lg;

  small_mode_symbol.width = W_MODE_SM;
  small_mode_symbol.height = H_MODE_SM;
  small_mode_symbol.y_offset = 0;
  small_mode_symbol.symbols = mode_sm;

  tft.init();
  tft.setRotation(TFT_SCREEN_ROTATION);
  tft.fillScreen(display_monitor.active_background_color);

  canvas.createSprite(TFT_WIDTH, TFT_HEIGHT);
  canvas.fillSprite(display_monitor.active_background_color);
  
  //high_voltage.createSprite(HV_WIDTH, HV_HEIGHT);
  //high_voltage.fillSprite(display_monitor.active_background_color);
  //high_voltage.setSwapBytes(true);
  
  //battery.createSprite(BATTERY_WIDTH, BATTERY_HEIGHT);
  //battery.fillSprite(display_monitor.active_background_color);
  //battery.setSwapBytes(true); 
  
  //diode.createSprite(DIODE_WIDTH, DIODE_HEIGHT);
  //diode.fillSprite(display_monitor.active_background_color);
  //diode.setSwapBytes(false); 

  zone_0.createSprite(ZONE_0_WIDTH, ZONE_0_HEIGHT);
  zone_0.fillSprite(display_monitor.active_background_color);
  zone_1.createSprite(ZONE_1_WIDTH, ZONE_1_HEIGHT);
  zone_1.fillSprite(display_monitor.active_background_color);
  zone_2.createSprite(ZONE_2_WIDTH, ZONE_2_HEIGHT);
  zone_2.fillSprite(display_monitor.active_background_color);
  zone_3.createSprite(ZONE_3_WIDTH, ZONE_3_HEIGHT);
  zone_3.fillSprite(display_monitor.active_background_color);
}


void display_monitor_tasks(void) {
  switch(display_monitor.state) {  

    case DISPLAY_MONITOR_STATE_INIT: 
      display_monitor.state = DISPLAY_MONITOR_STATE_WAIT;
      show_splash_screen(); 
      break;

    case DISPLAY_MONITOR_STATE_WAIT:
      canvas.pushSprite(0,0);
      if (!digits_monitor.in_strobe_phase) {
        display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_ZONE_0;
      }
      break;

    case DISPLAY_MONITOR_STATE_SHOW_ZONE_0:
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_MEASUREMENT_ZONE_1;
      show_zone_0();         
      break;

    case DISPLAY_MONITOR_STATE_SHOW_MEASUREMENT_ZONE_1:
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_ZONE_2;
      show_zone_1(); 
      break; 

    case DISPLAY_MONITOR_STATE_SHOW_ZONE_2: 
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_ZONE_3;
      show_zone_2(); 
      break;

    case DISPLAY_MONITOR_STATE_SHOW_ZONE_3: 
      display_monitor.state = DISPLAY_MONITOR_STATE_WAIT;
      show_zone_3();
      break;

    default:
      break; 
  }
}


void use_colors(unsigned int background_color, 
                unsigned int text_color) {
  if(display_monitor.active_background_color != background_color) {
    display_monitor.active_background_color = background_color; 
    canvas.fillSprite(background_color);
  }
  if(display_monitor.active_text_color != text_color) {
    display_monitor.active_text_color = text_color; 
    canvas.setTextColor(display_monitor.active_text_color, 
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

  unsigned int fg = display_monitor.active_text_color;
  unsigned int bg = display_monitor.active_background_color;
  if(INVERT_COLORS_SPLASH){
    fg = display_monitor.active_background_color;
    bg = display_monitor.active_text_color;
  }

  unsigned char x = (TFT_WIDTH - W_SPLASH_FLUKE)>>1; 
  unsigned char y = (TFT_HEIGHT - H_SPLASH_FLUKE)>>2;


  // First Splash Screen 
  //tft.drawBitmap(x, y, splashFluke, W_SPLASH_FLUKE, H_SPLASH_FLUKE, display_monitor.active_text_color, display_monitor.active_background_color);
  canvas.fillSprite(bg);
  canvas.drawBitmap(x, y, splashFluke, W_SPLASH_FLUKE, H_SPLASH_FLUKE, fg, bg);

  canvas.loadFont(AA_FONT_MEDIUM);

  canvas.setCursor(
    (TFT_WIDTH - canvas.textWidth(firmware_information.hardware))>>1, 
    y += H_SPLASH_FLUKE + canvas.fontHeight() +3
  );
  canvas.println(firmware_information.hardware);
  
  canvas.pushSprite(0,0); 

  delay(TFT_SPLASH_SCREEN_DURATION);   
  canvas.fillSprite(bg);


  // Second Splash Screen 
  y = ((TFT_HEIGHT - H_SPLASH_FLUKE)>>2) + 10;
  canvas.setCursor((TFT_WIDTH - canvas.textWidth(firmware_information.author))>>1, y);
  canvas.println(firmware_information.author);
  y += canvas.fontHeight() + 10; 
  canvas.unloadFont();
  canvas.loadFont(AA_FONT_SMALL);
  canvas.setCursor((TFT_WIDTH - canvas.textWidth(firmware_information.version))>>1, y);
  canvas.println(firmware_information.version);

  canvas.setCursor((TFT_WIDTH - canvas.textWidth(firmware_information.github1))>>1, y += canvas.fontHeight() + 5);
  canvas.println(firmware_information.github1);
  canvas.setCursor((TFT_WIDTH - canvas.textWidth(firmware_information.github2))>>1, y += canvas.fontHeight());
  canvas.println(firmware_information.github2);
  canvas.pushSprite(0,0);

  canvas.unloadFont();
  delay(TFT_SPLASH_SCREEN_DURATION<<1);   
  canvas.fillSprite(bg);
  canvas.pushSprite(0,0);
} 


void draw_using_one_symbol(TFT_eSprite &sprite, oneSymbol &one_symbol, bool invert_colors, unsigned char &x, unsigned char &y){
  unsigned int fg = display_monitor.active_text_color;
  unsigned int bg = display_monitor.active_background_color;
  if(invert_colors){
    fg = display_monitor.active_background_color;
    bg = display_monitor.active_text_color;
  }
  sprite.drawBitmap(x, y+one_symbol.y_offset, one_symbol.symbol, one_symbol.width, one_symbol.height, fg, bg);
  x += one_symbol.width;
}

void draw_using_array_of_symbols(TFT_eSprite &sprite, arrayOfSymbols &array_of_symbols, unsigned char d, bool invert_colors, unsigned char &x, unsigned char &y){
  unsigned int fg = display_monitor.active_text_color;
  unsigned int bg = display_monitor.active_background_color;
  if(invert_colors){
    fg = display_monitor.active_background_color;
    bg = display_monitor.active_text_color;
  }
  sprite.drawBitmap(x, y+array_of_symbols.y_offset, array_of_symbols.symbols [d], array_of_symbols.width, array_of_symbols.height, fg, bg);
  x += array_of_symbols.width;
}

void format_zone_1(void){

  unsigned char x = 0;
  unsigned char y = 0;

  if(contents_monitor.sign != NO_SIGN) {
    draw_using_array_of_symbols(zone_1, large_sign, contents_monitor.sign, INVERT_COLORS_SIGN_LG, x, y);
  } else {
    x += large_sign.width;
  }
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_ZERO){
    draw_using_one_symbol(zone_1, large_decimal_point, INVERT_COLORS_DIGIT_LG, x, y);
  } 
  draw_using_array_of_symbols(zone_1, large_digit, digits_monitor.st0_value0, INVERT_COLORS_DIGIT_LG, x, y);
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_ONE){
    draw_using_one_symbol(zone_1, large_decimal_point, INVERT_COLORS_DIGIT_LG, x, y);
  } 
  draw_using_array_of_symbols(zone_1, large_digit, digits_monitor.st1_value, INVERT_COLORS_DIGIT_LG, x, y);
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_TWO){
    draw_using_one_symbol(zone_1, large_decimal_point, INVERT_COLORS_DIGIT_LG, x, y);
  } 
  draw_using_array_of_symbols(zone_1, large_digit, digits_monitor.st2_value, INVERT_COLORS_DIGIT_LG, x, y);
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_THREE){
    draw_using_one_symbol(zone_1, large_decimal_point, INVERT_COLORS_DIGIT_LG, x, y);
  } 
  draw_using_array_of_symbols(zone_1, large_digit, digits_monitor.st3_value, INVERT_COLORS_DIGIT_LG, x, y);

  // add the unit symbol colors are reversed here
  x +=  REL_IN_ZONE_X_UNITS;
  y +=  REL_IN_ZONE_Y_UNITS;
  draw_using_array_of_symbols(zone_1, large_unit_symbol, contents_monitor.unit, INVERT_COLORS_UNIT_LG, x, y);
  x +=  REL_IN_ZONE_X_MODE;
  y +=  REL_IN_ZONE_Y_MODE;
  draw_using_array_of_symbols(zone_1, large_mode_symbol, contents_monitor.unit, INVERT_COLORS_MODE_LG, x, y);
}

void show_zone_1(){
  zone_1.fillSprite(display_monitor.active_background_color);
  format_zone_1();
  zone_1.pushToSprite(&canvas, X_ZONE_1, Y_ZONE_1);
}


void show_zone_0(void){
  zone_0.fillSprite(display_monitor.active_background_color);
  format_zone_0();
  zone_0.pushToSprite(&canvas, X_ZONE_0, Y_ZONE_0);
} 

void format_zone_2(void){}
void format_zone_3(void){}

void show_zone_2(void){}
void show_zone_3(void){}


void format_zone_0(void){
  unsigned char x = 0;
  unsigned char y = 0;

  zone_0.fillSprite(display_monitor.active_background_color);

  if(contents_monitor.battery != NO_BATTERY){
    show_battery();   
  }
  if(contents_monitor.high_voltage){
    show_high_voltage();
  }
  if(contents_monitor.diode_style != NO_DIODE){
    show_diode();
  }
  zone_0.pushToSprite(&canvas, X_ZONE_0, Y_ZONE_0);
}



void show_battery(void) {
  unsigned int fg = display_monitor.active_text_color;
  unsigned int bg = display_monitor.active_background_color;
  unsigned char x = REL_IN_ZONE_X_BATTERY;
  unsigned char y = REL_IN_ZONE_Y_BATTERY;

  if(INVERT_COLORS_BATTERY){
    fg = display_monitor.active_background_color;
    bg = display_monitor.active_text_color;
  }
  if (contents_monitor.battery == LOW_BATTERY) 
  {
    zone_0.drawBitmap(x,y,battery_low_symbol, BATTERY_WIDTH, BATTERY_HEIGHT, fg, bg);  
  } 
  else if  (contents_monitor.battery == NORMAL_BATTERY) 
  {
    zone_0.drawBitmap(x,y,battery_full_symbol, BATTERY_WIDTH, BATTERY_HEIGHT, fg, bg);  
  }
} 

void show_diode(void){
  unsigned int fg = display_monitor.active_text_color;
  unsigned int bg = display_monitor.active_background_color;
  unsigned char x = REL_IN_ZONE_X_DIODE;
  unsigned char y = REL_IN_ZONE_Y_DIODE;
  if(INVERT_COLORS_DIODE){
    fg = display_monitor.active_background_color;
    bg = display_monitor.active_text_color;
  }
  if (contents_monitor.diode_style != NO_DIODE) {
    zone_0.drawBitmap(x,y, diode_symbol, DIODE_WIDTH, DIODE_HEIGHT, fg, bg);  
  }
}


void show_high_voltage(void){
  unsigned int fg = display_monitor.active_text_color;
  unsigned int bg = display_monitor.active_background_color;
  unsigned char x = REL_IN_ZONE_X_HIGH_VOLTAGE;
  unsigned char y = REL_IN_ZONE_Y_HIGH_VOLTAGE;
  if(INVERT_COLORS_HV){
    fg = display_monitor.active_background_color;
    bg = display_monitor.active_text_color;
  }
  if(contents_monitor.high_voltage) {
    zone_0.drawBitmap(x,y,high_voltage_symbol, HV_WIDTH, HV_HEIGHT, fg, bg);
  }
}


void show_impedances(void){

}


