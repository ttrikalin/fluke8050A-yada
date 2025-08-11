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
  display_monitor.non_high_voltage_theme.voltage_background_color     = TFT_LIGHTGREEN;
  display_monitor.non_high_voltage_theme.voltage_text_color           = TFT_BLACK;
  display_monitor.non_high_voltage_theme.current_background_color     = TFT_SALMON;
  display_monitor.non_high_voltage_theme.current_text_color           = TFT_BLACK;
  display_monitor.non_high_voltage_theme.resistance_background_color  = TFT_LIGHTMUSTARD;
  display_monitor.non_high_voltage_theme.resistance_text_color        = TFT_BLACK;
  display_monitor.non_high_voltage_theme.conductance_background_color = TFT_LIGHTMUSTARD;
  display_monitor.non_high_voltage_theme.conductance_text_color       = TFT_BLACK;
  display_monitor.non_high_voltage_theme.gain_background_color        = TFT_WHITE;
  display_monitor.non_high_voltage_theme.gain_text_color              = TFT_BLACK;
  display_monitor.non_high_voltage_theme.invalid_background_color     = TFT_WHITE;
  display_monitor.non_high_voltage_theme.invalid_text_color           = TFT_RED;

  display_monitor.high_voltage_theme.splash_background_color      = TFT_DARKRED;
  display_monitor.high_voltage_theme.splash_text_color            = TFT_WHITE;
  display_monitor.high_voltage_theme.voltage_background_color     = TFT_DARKRED;
  display_monitor.high_voltage_theme.voltage_text_color           = TFT_WHITE;
  display_monitor.high_voltage_theme.current_background_color     = TFT_DARKRED;
  display_monitor.high_voltage_theme.current_text_color           = TFT_WHITE;
  display_monitor.high_voltage_theme.resistance_background_color  = TFT_DARKRED;
  display_monitor.high_voltage_theme.resistance_text_color        = TFT_WHITE;
  display_monitor.high_voltage_theme.conductance_background_color = TFT_DARKRED;
  display_monitor.high_voltage_theme.conductance_text_color       = TFT_WHITE;
  display_monitor.high_voltage_theme.gain_background_color        = TFT_DARKRED;
  display_monitor.high_voltage_theme.gain_text_color              = TFT_WHITE;
  display_monitor.high_voltage_theme.invalid_background_color     = TFT_WHITE;
  display_monitor.high_voltage_theme.invalid_text_color           = TFT_RED;

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


  battery_full.width = BATTERY_WIDTH;
  battery_full.height = BATTERY_HEIGHT;
  battery_full.y_offset = 0;
  battery_full.symbol = battery_full_symbol;
  
  battery_low.width = BATTERY_WIDTH;
  battery_low.height = BATTERY_HEIGHT;
  battery_low.y_offset = 0;
  battery_low.symbol = battery_low_symbol;

  diode.width = DIODE_WIDTH;
  diode.height = DIODE_HEIGHT;
  diode.y_offset = 0;
  diode.symbol = diode_symbol;

  high_voltage.width = HV_WIDTH;
  high_voltage.height = HV_HEIGHT;
  high_voltage.y_offset = 0;
  high_voltage.symbol = high_voltage_symbol;

  tft.init();
  tft.setRotation(TFT_SCREEN_ROTATION);
  tft.fillScreen(display_monitor.active_background_color);

  measurements_sprite.createSprite(MEASUREMENTS_WIDTH, MEASUREMENTS_HEIGHT);
  measurements_sprite.fillSprite(display_monitor.active_background_color);

  
}


void display_monitor_tasks(void) {
  switch(display_monitor.state) {  

    case DISPLAY_MONITOR_STATE_INIT: 
      display_monitor.state = DISPLAY_MONITOR_STATE_WAIT;
      draw_splash_screen(); 
      break;

    case DISPLAY_MONITOR_STATE_WAIT:
      //canvas.pushSprite(0,0);
      if (//!fast_tasks_monitor.isr_in_strobe_phase && 
          slow_tasks_monitor.new_values_flag) {
        display_monitor.state = DISPLAY_MONITOR_STATE_UPDATE_BACKGROUND_STATUS;
      } else {
        display_monitor.state = DISPLAY_MONITOR_STATE_UPDATE_MEASUREMENT;
      }
      break;

    case DISPLAY_MONITOR_STATE_UPDATE_BACKGROUND_STATUS:
      display_monitor.state = DISPLAY_MONITOR_STATE_UPDATE_MEASUREMENT;
      draw_background_status_screen();         
      slow_tasks_monitor.new_values_flag = false; 
      break;

    case DISPLAY_MONITOR_STATE_UPDATE_MEASUREMENT:
      display_monitor.state = DISPLAY_MONITOR_STATE_WAIT;
      draw_measurement(); 
      measurements_sprite.pushSprite(X_MEASUREMENTS, Y_MEASUREMENTS);
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
  if(fast_tasks_monitor.voltage_level == HIGH_VOLTAGE) {
    theme = &display_monitor.high_voltage_theme;
  } else {
    theme = &display_monitor.non_high_voltage_theme;
  } 
  
  if(slow_tasks_monitor.quantity == NO_QUANTITY) 
  {
    use_colors(theme->splash_background_color, 
                 theme->splash_text_color);
  } 
  else if (slow_tasks_monitor.quantity == RESISTANCE)
  {
    use_colors(theme->resistance_background_color, 
                 theme->resistance_text_color);
  }
  else if (slow_tasks_monitor.quantity == VOLTAGE)
  {
    use_colors(theme->voltage_background_color, 
                 theme->voltage_text_color);
  }
  else if (slow_tasks_monitor.quantity == CURRENT)
  {
    use_colors(theme->current_background_color, 
                 theme->current_text_color);
  } 
  else if (slow_tasks_monitor.quantity == CONDUCTANCE)
  {
    use_colors(theme->conductance_background_color, 
                 theme->conductance_text_color);
  }
  else if (slow_tasks_monitor.quantity == GAIN)
  {
    use_colors(theme->gain_background_color, 
                 theme->gain_text_color);
  }
}

void draw_splash_screen(void) {
  update_colors();

  unsigned int fg = display_monitor.active_text_color;
  unsigned int bg = display_monitor.active_background_color;
  if(INVERT_COLORS_SPLASH){
    fg = display_monitor.active_background_color;
    bg = display_monitor.active_text_color;
  }

  unsigned int x = (CANVAS_WIDTH - W_SPLASH_FLUKE)>>1;
  unsigned int y = (CANVAS_HEIGHT - H_SPLASH_FLUKE)>>2;


  // First Splash Screen 
  tft.fillScreen(bg);
  tft.drawBitmap(x, y, splashFluke, W_SPLASH_FLUKE, H_SPLASH_FLUKE, fg, bg);

  tft.loadFont(AA_FONT_MEDIUM);

  tft.setCursor(
    (CANVAS_WIDTH - tft.textWidth(firmware_information.hardware))>>1, 
    y += H_SPLASH_FLUKE + tft.fontHeight() +3
  );
  tft.println(firmware_information.hardware);
  

  delay(TFT_SPLASH_SCREEN_DURATION);   
  tft.fillScreen(bg);


  // Second Splash Screen 
  y = ((CANVAS_HEIGHT - H_SPLASH_FLUKE)>>2) + 10;
  tft.setCursor((CANVAS_WIDTH - tft.textWidth(firmware_information.author))>>1, y);
  tft.println(firmware_information.author);
  y += tft.fontHeight() + 10; 
  tft.unloadFont();
  tft.loadFont(AA_FONT_SMALL);
  tft.setCursor((CANVAS_WIDTH - tft.textWidth(firmware_information.version))>>1, y);
  tft.println(firmware_information.version);

  tft.setCursor((CANVAS_WIDTH - tft.textWidth(firmware_information.github1))>>1, y += tft.fontHeight() + 5);
  tft.println(firmware_information.github1);
  tft.setCursor((CANVAS_WIDTH - tft.textWidth(firmware_information.github2))>>1, y += tft.fontHeight());
  tft.println(firmware_information.github2);
  //tft.pushSprite(0,0);

  tft.unloadFont();
  delay(TFT_SPLASH_SCREEN_DURATION<<1);   
  tft.fillScreen(bg);
  //canvas.pushSprite(0,0);
} 


point draw_symbol_to_tft(TFT_eSPI &tft, oneSymbol &symbol, bool invert_colors, point p){
  unsigned int fg = display_monitor.active_text_color;
  unsigned int bg = display_monitor.active_background_color;
  if(invert_colors){
    fg = display_monitor.active_background_color;
    bg = display_monitor.active_text_color;
  }
  tft.drawBitmap(p.x, p.y+symbol.y_offset, symbol.symbol, symbol.width, symbol.height, fg, bg);
  return point{p.x+symbol.width, p.y};
}

point draw_symbol_to_sprite(TFT_eSprite &sprite, oneSymbol &symbol, bool invert_colors, point p){
  unsigned int fg = display_monitor.active_text_color;
  unsigned int bg = display_monitor.active_background_color;
  if(invert_colors){
    fg = display_monitor.active_background_color;
    bg = display_monitor.active_text_color;
  }
  sprite.drawBitmap(p.x, p.y+symbol.y_offset, symbol.symbol, symbol.width, symbol.height, fg, bg);
  return point{p.x+symbol.width, p.y};
}

point draw_symbol_array_element_to_sprite(TFT_eSprite &sprite, arrayOfSymbols &array_of_symbols, unsigned int d, bool invert_colors, point p){
  unsigned int fg = display_monitor.active_text_color;
  unsigned int bg = display_monitor.active_background_color;
  if(invert_colors){
    fg = display_monitor.active_background_color;
    bg = display_monitor.active_text_color;
  }
  sprite.drawBitmap(p.x, p.y+array_of_symbols.y_offset, array_of_symbols.symbols [d], array_of_symbols.width, array_of_symbols.height, fg, bg);
  return point{p.x+array_of_symbols.width, p.y};
}

point draw_symbol_array_element_to_tft(TFT_eSPI &tft, arrayOfSymbols &array_of_symbols, unsigned int d, bool invert_colors, point p){
  unsigned int fg = display_monitor.active_text_color;
  unsigned int bg = display_monitor.active_background_color;
  if(invert_colors){
    fg = display_monitor.active_background_color;
    bg = display_monitor.active_text_color;
  }
  tft.drawBitmap(p.x, p.y+array_of_symbols.y_offset, array_of_symbols.symbols [d], array_of_symbols.width, array_of_symbols.height, fg, bg);
  return point{p.x+array_of_symbols.width, p.y};
}

// void draw_battery(void) {
//   unsigned int fg = display_monitor.active_text_color;
//   unsigned int bg = display_monitor.active_background_color;
//   unsigned int x = X_BATTERY;
//   unsigned int y = Y_BATTERY;

//   if(INVERT_COLORS_BATTERY){
//     fg = display_monitor.active_background_color;
//     bg = display_monitor.active_text_color;
//   }
//   if (slow_tasks_monitor.battery == LOW_BATTERY) 
//   {
//     tft.drawBitmap(x,y,battery_low_symbol, BATTERY_WIDTH, BATTERY_HEIGHT, fg, bg);  
//   } 
//   else if  (slow_tasks_monitor.battery == NORMAL_BATTERY) 
//   {
//     tft.drawBitmap(x,y,battery_full_symbol, BATTERY_WIDTH, BATTERY_HEIGHT, fg, bg);  
//   }
// } 

// void draw_diode(void){
//   unsigned int fg = display_monitor.active_text_color;
//   unsigned int bg = display_monitor.active_background_color;
//   unsigned int x = X_DIODE;
//   unsigned int y = Y_DIODE;
//   if(INVERT_COLORS_DIODE){
//     fg = display_monitor.active_background_color;
//     bg = display_monitor.active_text_color;
//   }
//   if (slow_tasks_monitor.diode_style != NO_DIODE) {
//     tft.drawBitmap(x,y, diode_symbol, DIODE_WIDTH, DIODE_HEIGHT, fg, bg);  
//   }
// }

// void draw_icon(TFT_eSPSI &tft, const unsigned int x, const unsigned int y, const &symbol, const unsigned int width, unsigned int height, bool invert_colors){
//   unsigned int fg = display_monitor.active_text_color;
//   unsigned int bg = display_monitor.active_background_color;
//   if(invert_colors){
//     fg = display_monitor.active_background_color;
//     bg = display_monitor.active_text_color;
//   }
//   tft.drawBitmap(x,y,symbol, width, height, fg, bg);
// }

// void draw_high_voltage(unsigned int x, unsigned int y){
//   unsigned int fg = display_monitor.active_text_color;
//   unsigned int bg = display_monitor.active_background_color;
//   //unsigned int x = REL_IN_ZONE_X_HIGH_VOLTAGE;
//   //unsigned int y = REL_IN_ZONE_Y_HIGH_VOLTAGE;
//   //if(true ){//||fast_tasks_monitor.voltage_level == HIGH_VOLTAGE) {
//     if(INVERT_COLORS_HV){
//       fg = display_monitor.active_background_color;
//       bg = display_monitor.active_text_color;
//     }
//     measurements_sprite.drawBitmap(x,y,high_voltage_symbol, HV_WIDTH, HV_HEIGHT, fg, bg);
//   //}
// }




void draw_background_status_screen(void){
  update_colors();
  
  point p = {X_BATTERY, Y_BATTERY};
  tft.fillScreen(display_monitor.active_background_color);
  if(slow_tasks_monitor.battery == NORMAL_BATTERY){
    draw_symbol_to_tft(tft, battery_full, INVERT_COLORS_BATTERY, p);
  } else if(slow_tasks_monitor.battery == LOW_BATTERY){
    draw_symbol_to_tft(tft, battery_low, INVERT_COLORS_BATTERY, p);
  }
  if(slow_tasks_monitor.diode_style != NO_DIODE){
    p.x = X_DIODE;
    p.y = Y_DIODE;
    draw_symbol_to_tft(tft, diode, INVERT_COLORS_DIODE, p);
  }
  if(slow_tasks_monitor.unit != NO_UNIT){ 
    p.x = X_UNITS;
    p.y = Y_UNITS;
    draw_symbol_array_element_to_tft(tft, large_unit_symbol, slow_tasks_monitor.unit, INVERT_COLORS_UNIT, p);
  }
  if(slow_tasks_monitor.acdc_mode != NO_ACDC){
    p.x = X_MODE;
    p.y = Y_MODE;
    draw_symbol_array_element_to_tft(tft, large_mode_symbol , slow_tasks_monitor.acdc_mode, INVERT_COLORS_MODE, p);
  }
}



void draw_measurement(void){

  point p = {0, 0};

  measurements_sprite.fillSprite(display_monitor.active_background_color);

  if(fast_tasks_monitor.voltage_level == HIGH_VOLTAGE){
    p = draw_symbol_to_sprite(measurements_sprite, high_voltage, INVERT_COLORS_HV, p);
  } else {
    p.x += HV_WIDTH; 
  }

  if(fast_tasks_monitor.sign != NO_SIGN) {
    p = draw_symbol_array_element_to_sprite(measurements_sprite, large_sign, fast_tasks_monitor.sign, INVERT_COLORS_SIGN_LG, p);
  } else {
    p.x += large_sign.width;
  }
  if(slow_tasks_monitor.decimal_point_position == DECIMAL_POINT_AT_ZERO){
    p = draw_symbol_to_sprite(measurements_sprite, large_decimal_point, INVERT_COLORS_DIGIT_LG, p);
  } 
  p = draw_symbol_array_element_to_sprite(measurements_sprite, large_digit, fast_tasks_monitor.st0_value0, INVERT_COLORS_DIGIT_LG, p);
  if(slow_tasks_monitor.decimal_point_position == DECIMAL_POINT_AT_ONE){
    p = draw_symbol_to_sprite(measurements_sprite, large_decimal_point, INVERT_COLORS_DIGIT_LG, p);
  } 
  p = draw_symbol_array_element_to_sprite(measurements_sprite, large_digit, fast_tasks_monitor.st1_value, INVERT_COLORS_DIGIT_LG, p);
  if(slow_tasks_monitor.decimal_point_position == DECIMAL_POINT_AT_TWO){
    p = draw_symbol_to_sprite(measurements_sprite, large_decimal_point, INVERT_COLORS_DIGIT_LG, p);
  } 
  p = draw_symbol_array_element_to_sprite(measurements_sprite, large_digit, fast_tasks_monitor.st2_value, INVERT_COLORS_DIGIT_LG, p);
  if(slow_tasks_monitor.decimal_point_position == DECIMAL_POINT_AT_THREE){
    p = draw_symbol_to_sprite(measurements_sprite, large_decimal_point, INVERT_COLORS_DIGIT_LG, p);
  } 
  p = draw_symbol_array_element_to_sprite(measurements_sprite, large_digit, fast_tasks_monitor.st3_value, INVERT_COLORS_DIGIT_LG, p);

  // add the unit symbol and mode symbol
  //x +=  REL_IN_ZONE_X_UNITS;
  //y +=  REL_IN_ZONE_Y_UNITS;
  p = draw_symbol_array_element_to_sprite(measurements_sprite, large_unit_symbol, slow_tasks_monitor.unit, INVERT_COLORS_UNIT, p);
  //x += large_unit_symbol.width + 2;
  //x +=  REL_IN_ZONE_X_MODE - REL_IN_ZONE_X_UNITS - W_UNIT_LG;
  //y +=  REL_IN_ZONE_Y_MODE - REL_IN_ZONE_Y_UNITS;
  p = draw_symbol_array_element_to_sprite(measurements_sprite, small_mode_symbol, slow_tasks_monitor.acdc_mode, INVERT_COLORS_MODE, p);
}






