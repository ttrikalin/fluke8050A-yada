#include "TFT_8050A.h"



extern displayMonitorData display_monitor;
extern TFT_eSPI tft;

extern TFT_eSprite measurements_sprite;
extern TFT_eSprite analog_meter_sprite;
 
extern FastTasksMonitorData fast_tasks_monitor;
extern InputsMonitorData inputs_monitor;  
extern oneSymbol large_decimal_point; 
extern oneSymbol small_decimal_point; 
extern oneSymbol battery_full; 
extern oneSymbol battery_low; 
extern oneSymbol diode; 
extern oneSymbol high_voltage;
extern arrayOfSymbols large_sign; 
extern arrayOfSymbols small_sign; 
extern arrayOfSymbols large_digit; 
extern arrayOfSymbols small_digit; 
extern arrayOfSymbols large_unit_symbol; 
extern arrayOfSymbols small_unit_symbol; 
extern arrayOfSymbols large_mode_symbol; 
extern arrayOfSymbols small_mode_symbol; 

void display_monitor_initialize(void) {
  display_monitor.state = DISPLAY_MONITOR_STATE_INIT;

  display_monitor.active_background_color = TFT_BLACK;
  display_monitor.active_text_color = TFT_WHITE;
  display_monitor.active_negative_meter_color = TFT_RED;

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
  display_monitor.non_high_voltage_theme.negative_meter_color         = TFT_RED;

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
  display_monitor.high_voltage_theme.negative_meter_color         = TFT_BLACK;

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

  analog_meter_sprite.createSprite(ANALOG_METER_WIDTH, ANALOG_METER_HEIGHT);
  analog_meter_sprite.fillSprite(display_monitor.active_background_color);
}


void display_monitor_tasks(void) {
  switch(display_monitor.state) {  

    case DISPLAY_MONITOR_STATE_INIT: 
      display_monitor.state = DISPLAY_MONITOR_STATE_WAIT;
      draw_splash_screen(); 
      break;

    case DISPLAY_MONITOR_STATE_WAIT:
      if (fast_tasks_monitor.isr_active_strobe != NO_STROBE) break;
      #ifdef DEBUG_ENABLE
        display_monitor.state = DISPLAY_MONITOR_STATE_DEBUG_SCREEN;
      #else
        if (!inputs_monitor.valid_input_combination) {
          display_monitor.state = DISPLAY_MONITOR_STATE_INVALID_INPUTS;
          break;
        }
        if (inputs_monitor.new_values_flag) {
          display_monitor.state = DISPLAY_MONITOR_STATE_UPDATE_BACKGROUND_STATUS;
        } else {
          display_monitor.state = DISPLAY_MONITOR_STATE_UPDATE_MEASUREMENT;
        }
      #endif
      break;

    case DISPLAY_MONITOR_STATE_UPDATE_BACKGROUND_STATUS:
      display_monitor.state = DISPLAY_MONITOR_STATE_UPDATE_MEASUREMENT;
      draw_background_status_screen();         
      inputs_monitor.new_values_flag = false; 
      break;

    case DISPLAY_MONITOR_STATE_UPDATE_MEASUREMENT:
      display_monitor.state = DISPLAY_MONITOR_STATE_WAIT;
      //draw_measurement(); 
      measurements_sprite.pushSprite(X_MEASUREMENTS, Y_MEASUREMENTS);
      draw_analog_meter(inputs_monitor.function == Function::CURRENT || inputs_monitor.function == Function::VOLTAGE);
      analog_meter_sprite.pushSprite(X_ANALOG_METER, Y_ANALOG_METER);
      break; 

    case DISPLAY_MONITOR_STATE_INVALID_INPUTS:
      display_monitor.state = DISPLAY_MONITOR_STATE_WAIT;  
      draw_invalid_inputs_screen();
      break;

    case DISPLAY_MONITOR_STATE_DEBUG_SCREEN:
      display_monitor.state = DISPLAY_MONITOR_STATE_WAIT;  
      draw_debug_screen();
      break;

    default:
      break; 
  }
}

void use_colors(unsigned int background_color, 
                unsigned int text_color,
                unsigned int negative_meter_color) {
  // Check if colors actually changed before updating
  if(display_monitor.active_background_color != background_color ||
     display_monitor.active_text_color != text_color ||
     display_monitor.active_negative_meter_color != negative_meter_color) {
    display_monitor.active_background_color = background_color; 
    display_monitor.active_text_color = text_color;
    display_monitor.active_negative_meter_color = negative_meter_color;
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
  
  if(inputs_monitor.base_unit == BaseUnit::INVALID) 
  {
    use_colors(theme->splash_background_color, 
                 theme->splash_text_color, 
                 theme->negative_meter_color);
  } 
  else if (inputs_monitor.base_unit == BaseUnit::OHM)
  {
    use_colors(theme->resistance_background_color, 
                 theme->resistance_text_color,
                 theme->negative_meter_color);
  }
  else if (inputs_monitor.base_unit == BaseUnit::VOLT)
  {
    use_colors(theme->voltage_background_color, 
                 theme->voltage_text_color,
                 theme->negative_meter_color);
  }
  else if (inputs_monitor.base_unit == BaseUnit::AMPERE)
  {
    use_colors(theme->current_background_color, 
                 theme->current_text_color,
                 theme->negative_meter_color);
  } 
  else if (inputs_monitor.base_unit == BaseUnit::SIEMENS)
  {
    use_colors(theme->conductance_background_color, 
                 theme->conductance_text_color,
                 theme->negative_meter_color);
  }
  else if (inputs_monitor.base_unit == BaseUnit::DECIBEL || 
           inputs_monitor.base_unit == BaseUnit::IMPEDANCE_Z)
  {
    use_colors(theme->gain_background_color, 
                 theme->gain_text_color,
                 theme->negative_meter_color);
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



void draw_background_status_screen(void){
  update_colors();
  
  point p = {X_BATTERY, Y_BATTERY};
  tft.fillScreen(display_monitor.active_background_color);
  if(inputs_monitor.battery == BatteryStyle::NORMAL_BATTERY){
    draw_symbol_to_tft(tft, battery_full, INVERT_COLORS_BATTERY, p);
  } else if(inputs_monitor.battery == BatteryStyle::LOW_BATTERY){
    draw_symbol_to_tft(tft, battery_low, INVERT_COLORS_BATTERY, p);
  }
  if(inputs_monitor.diode_style != DiodeStyle::NO_DIODE){
    p.x = X_DIODE;
    p.y = Y_DIODE;
    draw_symbol_to_tft(tft, diode, INVERT_COLORS_DIODE, p);
  }
  if(inputs_monitor.unit != Unit::NO_UNIT){ 
    p.x = X_UNITS;
    p.y = Y_UNITS;
    draw_symbol_array_element_to_tft(tft, large_unit_symbol, (unsigned int) inputs_monitor.unit, INVERT_COLORS_UNIT, p);
  }
  if(inputs_monitor.acdc_mode != Mode::NO_ACDC){
    p.x = X_MODE;
    p.y = Y_MODE;
    draw_symbol_array_element_to_tft(tft, large_mode_symbol , (unsigned int) inputs_monitor.acdc_mode, 
      INVERT_COLORS_MODE, p);
  }
}



void draw_measurement(void){

  point p = {0, 0};


  if(fast_tasks_monitor.voltage_level == HIGH_VOLTAGE){
    update_colors();
    measurements_sprite.fillSprite(display_monitor.active_background_color);
    p = draw_symbol_to_sprite(measurements_sprite, high_voltage, INVERT_COLORS_HV, p);
  } else {
    p.x += HV_WIDTH; 
    measurements_sprite.fillSprite(display_monitor.active_background_color);
  }

  if(fast_tasks_monitor.sign != Sign::NO_SIGN) {
    p = draw_symbol_array_element_to_sprite(measurements_sprite, large_sign, (unsigned int)fast_tasks_monitor.sign, 
      INVERT_COLORS_SIGN_LG, p);
    p.x += large_sign.width;
  } else {
    p.x += 2*large_sign.width;
  }
  if(inputs_monitor.decimal_point_position == DecimalPointPosition::DECIMAL_POINT_AT_ZERO){
    p = draw_symbol_to_sprite(measurements_sprite, large_decimal_point, INVERT_COLORS_DIGIT_LG, p);
  } 
  // Bounds check: digit values are 4-bit (0-15), but arrays only have 0-9
  unsigned int d0 = (fast_tasks_monitor.st0_value0 > 9) ? 0 : fast_tasks_monitor.st0_value0;
  unsigned int d1 = (fast_tasks_monitor.st1_value > 9) ? 0 : fast_tasks_monitor.st1_value;
  unsigned int d2 = (fast_tasks_monitor.st2_value > 9) ? 0 : fast_tasks_monitor.st2_value;
  unsigned int d3 = (fast_tasks_monitor.st3_value > 9) ? 0 : fast_tasks_monitor.st3_value;
  
  p = draw_symbol_array_element_to_sprite(measurements_sprite, large_digit, d0, INVERT_COLORS_DIGIT_LG, p);
  if(inputs_monitor.decimal_point_position == DecimalPointPosition::DECIMAL_POINT_AT_ONE){
    p = draw_symbol_to_sprite(measurements_sprite, large_decimal_point, INVERT_COLORS_DIGIT_LG, p);
  } 
  p = draw_symbol_array_element_to_sprite(measurements_sprite, large_digit, d1, INVERT_COLORS_DIGIT_LG, p);
  if(inputs_monitor.decimal_point_position == DecimalPointPosition::DECIMAL_POINT_AT_TWO){
    p = draw_symbol_to_sprite(measurements_sprite, large_decimal_point, INVERT_COLORS_DIGIT_LG, p);
  } 
  p = draw_symbol_array_element_to_sprite(measurements_sprite, large_digit, d2, INVERT_COLORS_DIGIT_LG, p);
  if(inputs_monitor.decimal_point_position == DecimalPointPosition::DECIMAL_POINT_AT_THREE){
    p = draw_symbol_to_sprite(measurements_sprite, large_decimal_point, INVERT_COLORS_DIGIT_LG, p);
  } 
  p = draw_symbol_array_element_to_sprite(measurements_sprite, large_digit, d3, INVERT_COLORS_DIGIT_LG, p);

  // add the unit symbol and mode symbol
  //x +=  REL_IN_ZONE_X_UNITS;
  //y +=  REL_IN_ZONE_Y_UNITS;
  // Bounds check: Unit enum has NO_UNIT=-1, valid values are 0-10
  if(inputs_monitor.unit != Unit::NO_UNIT) {
    unsigned int unit_idx = (unsigned int)inputs_monitor.unit;
    if(unit_idx <= 10) {  // Unit enum max is IMPEDANCE_Z=10
      p = draw_symbol_array_element_to_sprite(measurements_sprite, large_unit_symbol, unit_idx, 
        INVERT_COLORS_UNIT, p);
    }
  }
  //x += large_unit_symbol.width + 2;
  //x +=  REL_IN_ZONE_X_MODE - REL_IN_ZONE_X_UNITS - W_UNIT_LG;
  //y +=  REL_IN_ZONE_Y_MODE - REL_IN_ZONE_Y_UNITS;
  // Bounds check: Mode enum has NO_ACDC=-1, valid values are 0-1
  if(inputs_monitor.acdc_mode != Mode::NO_ACDC) {
    unsigned int mode_idx = (unsigned int)inputs_monitor.acdc_mode;
    if(mode_idx <= 1) {  // Mode enum max is AC=1
      p = draw_symbol_array_element_to_sprite(measurements_sprite, small_mode_symbol, mode_idx, 
        INVERT_COLORS_MODE, p);
    }
  }
}



void draw_analog_meter(bool is_signed){
  analog_meter_sprite.fillSprite(display_monitor.active_background_color);

  int y = 0;
  int h = (ANALOG_METER_HEIGHT);
  int x = 0;
  int w = 0;
  if(is_signed){
    x = (ANALOG_METER_WIDTH>>1);
    w =   (int) 
            ( (ANALOG_METER_WIDTH>>1) * ((fast_tasks_monitor.st0_value0 * 1000) + 
            (fast_tasks_monitor.st1_value * 100) + 
            (fast_tasks_monitor.st2_value * 10) + 
            fast_tasks_monitor.st3_value) / 1999.0f);
    if(fast_tasks_monitor.sign == Sign::POSITIVE_SIGN){
      analog_meter_sprite.fillRect(x, y , w, h , display_monitor.active_text_color);
    } else {
      analog_meter_sprite.fillRect(x-w, y, w, h , display_monitor.active_negative_meter_color);
    }
  } else {
    w = (int) 
            ( ANALOG_METER_WIDTH * ((fast_tasks_monitor.st0_value0 * 1000) + 
            (fast_tasks_monitor.st1_value * 100) + 
            (fast_tasks_monitor.st2_value * 10) + 
            fast_tasks_monitor.st3_value) / 1999.0f);
    analog_meter_sprite.fillRect(x, y , w, h , display_monitor.active_text_color);
  }
  
  
  analog_meter_sprite.drawRect(0, 0, ANALOG_METER_WIDTH, ANALOG_METER_HEIGHT, display_monitor.active_text_color);
  analog_meter_sprite.drawLine(x, y, x, y+h, display_monitor.active_text_color); 

}


void draw_invalid_inputs_screen (void) {
  tft.fillScreen(display_monitor.active_background_color);
  tft.loadFont(AA_FONT_SMALL);
  tft.setCursor((CANVAS_WIDTH - tft.textWidth(firmware_information.hardware))>>1, tft.fontHeight() +3);
  tft.println("Invalid Input\nCombination!");
  tft.print("ST0_0=");
  tft.print((unsigned int)fast_tasks_monitor.st0_value0);
  tft.print(" | ST0_1=");
  tft.print((unsigned int)fast_tasks_monitor.st0_value1);
  tft.print(" | ST0_2=");
  tft.println((unsigned int)fast_tasks_monitor.st0_value2);
  tft.print("ST1=");
  tft.println((unsigned int)fast_tasks_monitor.st1_value);
  tft.print("ST2=");
  tft.println((unsigned int)fast_tasks_monitor.st2_value);
  tft.print("ST3=");
  tft.println((unsigned int)fast_tasks_monitor.st3_value);
  tft.unloadFont();
}


void draw_debug_screen (void) {
  tft.fillScreen(display_monitor.active_background_color);
  tft.loadFont(AA_FONT_SMALL);
  tft.setCursor(0, tft.fontHeight() +3);
  tft.println("Debug Information");
  tft.print("ST0_0=");
  tft.print((unsigned int)fast_tasks_monitor.st0_value0);
  tft.print(" | ST0_1=");
  tft.print((unsigned int)fast_tasks_monitor.st0_value1);
  tft.print(" | ST0_2=");
  tft.println((unsigned int)fast_tasks_monitor.st0_value2);
  tft.print("ST1=");
  tft.println((unsigned int)fast_tasks_monitor.st1_value);
  tft.print("ST2=");
  tft.println((unsigned int)fast_tasks_monitor.st2_value);
  tft.print("ST3=");
  tft.println((unsigned int)fast_tasks_monitor.st3_value);

  tft.print("RNG_A=");
  tft.print((unsigned int)digitalRead(fluke8050a_RNG_A));
  tft.print(" | RNG_B=");
  tft.print((unsigned int)digitalRead(fluke8050a_RNG_B));
  tft.print(" | RNG_C=");
  tft.print((unsigned int)digitalRead(fluke8050a_RNG_C));
  tft.print(" == ");
  tft.println((unsigned int) inputs_monitor.range);


  tft.print("F_A=");
  tft.print((unsigned int)digitalRead(fluke8050a_FUNC_A));
  tft.print(" | F_B=");
  tft.print((unsigned int)digitalRead(fluke8050a_FUNC_B));
  tft.print(" | F_C=");
  tft.print((unsigned int)digitalRead(fluke8050a_FUNC_C));
  tft.print(" | F_D=");
  tft.print((unsigned int)digitalRead(fluke8050a_FUNC_D));
  tft.print(" == ");
  tft.println((unsigned int) inputs_monitor.function);

  tft.print("BattLow=");
  tft.println((unsigned int)digitalRead(fluke8050a_BT));

  tft.print("HV="); 
  tft.println((unsigned int)digitalRead(fluke8050a_HV)); 
  
  tft.print("DP="); 
  tft.println((unsigned int)digitalRead(fluke8050a_DP)); 

  tft.print("W=");
  tft.print((unsigned int)digitalRead(fluke8050a_W));
  tft.print(" | X=");
  tft.print((unsigned int)digitalRead(fluke8050a_X));
  tft.print(" | Y=");
  tft.print((unsigned int)digitalRead(fluke8050a_Y));
  tft.print(" | Z=");
  tft.println((unsigned int)digitalRead(fluke8050a_Z));

  tft.unloadFont();
}