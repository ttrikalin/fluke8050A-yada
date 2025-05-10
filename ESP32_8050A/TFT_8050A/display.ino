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


  tft.init();
  tft.setRotation(TFT_SCREEN_ROTATION);
  tft.fillScreen(display_monitor.active_background_color);

  canvas.createSprite(TFT_WIDTH, TFT_HEIGHT);
  canvas.fillSprite(display_monitor.active_background_color);
  
  high_voltage.createSprite(HV_WIDTH, HV_HEIGHT);
  high_voltage.fillSprite(display_monitor.active_background_color);
  //high_voltage.setSwapBytes(true);
  
  battery.createSprite(BATTERY_WIDTH, BATTERY_HEIGHT);
  battery.fillSprite(display_monitor.active_background_color);
  //battery.setSwapBytes(true); 
  
  diode.createSprite(DIODE_WIDTH, DIODE_HEIGHT);
  diode.fillSprite(display_monitor.active_background_color);
  //diode.setSwapBytes(false); 

  zone_one.createSprite(ZONE_ONE_WIDTH, ZONE_ONE_HEIGHT);
  zone_one.fillSprite(display_monitor.active_background_color);
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
        display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_HIGH_VOLTAGE;
      }
      break;

    case DISPLAY_MONITOR_STATE_SHOW_HIGH_VOLTAGE:
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_VALUE;
      show_high_voltage();         
      break;

    case DISPLAY_MONITOR_STATE_SHOW_VALUE:
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_REFERENCE_VALUE;
      show_zone_one(); 
      break; 

    case DISPLAY_MONITOR_STATE_SHOW_REFERENCE_VALUE: 
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_LOW_BATTERY;
      //show_reference_value(); 
      break;

    case DISPLAY_MONITOR_STATE_SHOW_LOW_BATTERY: 
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_DIODE;
      if(contents_monitor.battery != NO_BATTERY){
        show_battery();   
      }
      break;

    case DISPLAY_MONITOR_STATE_SHOW_DIODE:
      display_monitor.state = DISPLAY_MONITOR_STATE_SHOW_IMPEDANCES;
      show_diode(); 
      break;

    case DISPLAY_MONITOR_STATE_SHOW_IMPEDANCES:
      display_monitor.state = DISPLAY_MONITOR_STATE_WAIT;
      //show_impedances(); 
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
  unsigned char x = (TFT_WIDTH - W_SPLASH_FLUKE)>>1; 
  unsigned char y = (TFT_HEIGHT - H_SPLASH_FLUKE)>>2;


  // First Splash Screen 
  //tft.drawBitmap(x, y, splashFluke, W_SPLASH_FLUKE, H_SPLASH_FLUKE, display_monitor.active_text_color, display_monitor.active_background_color);
  canvas.fillSprite(display_monitor.active_background_color);
  canvas.drawBitmap(x, y, splashFluke, W_SPLASH_FLUKE, H_SPLASH_FLUKE, display_monitor.active_text_color, display_monitor.active_background_color);

  canvas.loadFont(AA_FONT_MEDIUM);

  canvas.setCursor(
    (TFT_WIDTH - canvas.textWidth(firmware_information.hardware))>>1, 
    y += H_SPLASH_FLUKE + canvas.fontHeight() +3
  );
  canvas.println(firmware_information.hardware);
  
  canvas.pushSprite(0,0); 

  delay(TFT_SPLASH_SCREEN_DURATION);   
  canvas.fillSprite(display_monitor.active_background_color);


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
  canvas.fillSprite(display_monitor.active_background_color);
  canvas.pushSprite(0,0);
} 





// void draw_large_sign(signs sign, unsigned char &x, unsigned char &y) {
//   if(sign != NO_SIGN) {
//     zone_one.drawBitmap(x, y, sign_lg[contents_monitor.sign], 
//       W_SIGN_LG, H_SIGN_LG, 
//       display_monitor.active_background_color, 
//       display_monitor.active_text_color
//       );  
//   }
//   x += W_SIGN_LG;
// }


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



void format_large_digits(void){

  unsigned char x = 0;
  unsigned char y = 0;

  if(contents_monitor.sign != NO_SIGN) {
    draw_using_array_of_symbols(zone_one, large_sign, contents_monitor.sign, true, x, y);
  } else {
    x += large_sign.width;
    //draw_using_array_of_symbols(zone_one, large_sign, 0, x, y);
  }
  //draw_large_sign(contents_monitor.sign, x, y);
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_ZERO){
    draw_using_one_symbol(zone_one, large_decimal_point, true, x, y);
  } 
  //draw_using_array_of_symbols(zone_one, large_unit_symbol, contents_monitor.unit, false, x, y);
  draw_using_array_of_symbols(zone_one, large_digit, digits_monitor.st0_value0, true, x, y);
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_ONE){
    draw_using_one_symbol(zone_one, large_decimal_point, true, x, y);
  } 
  draw_using_array_of_symbols(zone_one, large_digit, digits_monitor.st1_value, true, x, y);
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_TWO){
    draw_using_one_symbol(zone_one, large_decimal_point, true, x, y);
  } 
  draw_using_array_of_symbols(zone_one, large_digit, digits_monitor.st2_value, true, x, y);
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_THREE){
    draw_using_one_symbol(zone_one, large_decimal_point, true, x, y);
  } 
  draw_using_array_of_symbols(zone_one, large_digit, digits_monitor.st3_value, true, x, y);

  draw_using_array_of_symbols(zone_one, large_unit_symbol, contents_monitor.unit, false, x, y);
}

void show_zone_one(void){
  zone_one.fillSprite(display_monitor.active_background_color);
  format_large_digits();
  zone_one.pushToSprite(&canvas, X_ZONE_ONE, Y_ZONE_ONE);
}
void show_reference_value(void){
  update_colors();
  tft.loadFont(AA_FONT_SMALL);
  tft.setCursor(X_REL, Y_REL);
  tft.println("9999");
} 

// void show_unit(void) {
//   //unsigned char x = 0;
//   //unsigned char y = 0;
  
//   //draw_using_array_of_symbols(zone_one, large_unit_symbol, contents_monitor.unit, false, x, y);
// }

void show_battery(void) {
  battery.setBitmapColor(display_monitor.active_text_color, display_monitor.active_background_color);
  battery.fillSprite(display_monitor.active_background_color);
  if (contents_monitor.battery == LOW_BATTERY) 
  {
    battery.drawBitmap(0,0,battery_low_symbol, BATTERY_WIDTH, BATTERY_HEIGHT, display_monitor.active_background_color, display_monitor.active_text_color);  
  } 
  else if  (contents_monitor.battery == NORMAL_BATTERY) 
  {
    battery.drawBitmap(0,0,battery_full_symbol, BATTERY_WIDTH, BATTERY_HEIGHT, display_monitor.active_background_color, display_monitor.active_text_color);  
  }
  battery.pushToSprite(&canvas, X_BATTERY, Y_BATTERY);
} 

void show_diode(void){
  diode.fillSprite(display_monitor.active_background_color);
  if (contents_monitor.diode_style != NO_DIODE) {
    diode.drawBitmap(0,0, diode_symbol, DIODE_WIDTH, DIODE_HEIGHT, display_monitor.active_background_color, display_monitor.active_text_color);  
  }
  diode.pushToSprite(&canvas, X_DIODE, Y_DIODE);
}


void show_high_voltage(void){
  update_colors();
  high_voltage.fillSprite(display_monitor.active_background_color);
  if(contents_monitor.high_voltage) {
    high_voltage.drawBitmap(0,0,high_voltage_symbol, HV_WIDTH, HV_HEIGHT,  display_monitor.active_background_color, display_monitor.active_text_color);
  }
  high_voltage.pushToSprite(&canvas, 0, 0);
}


void show_impedances(void){

}


