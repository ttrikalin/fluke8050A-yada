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

void format_digits(void){
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_ZERO){
    display_monitor.digits_str = "." + digits_monitor.st0_value0; 
  } 
  else {
    display_monitor.digits_str = digits_monitor.st0_value0;   
  }
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_ONE){
    display_monitor.digits_str += "."; 
  }
  display_monitor.digits_str += digits_monitor.st1_value;
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_TWO){
    display_monitor.digits_str += "."; 
  }
  display_monitor.digits_str += digits_monitor.st2_value;
  if(contents_monitor.decimal_point_position == DECIMAL_POINT_AT_THREE){
    display_monitor.digits_str += "."; 
  }
  display_monitor.digits_str += digits_monitor.st3_value;

  if (contents_monitor.sign == POSITIVE_SIGN) 
  {
    display_monitor.digits_str = "+" + display_monitor.digits_str;
  } 
  else if (contents_monitor.sign == NEGATIVE_SIGN) 
  {
    display_monitor.digits_str = "-" + display_monitor.digits_str;
  }
  else {
    display_monitor.digits_str = " " + display_monitor.digits_str;
  }
}

void show_high_voltage(void){
  update_colors();
  tft.setCursor(0, 0);
  tft.setTextPadding(tft.textWidth("HV!"));
  if(contents_monitor.high_voltage) {
    tft.println("HV!");
  } else {
    tft.println("");
  }
}

void show_digits(void){
  update_colors();
  tft.loadFont(AA_FONT_MEDIUM);
  format_digits();
  unsigned char x = (TFT_WIDTH - tft.textWidth(display_monitor.digits_str))>>2;
  unsigned char y = tft.fontHeight();

  tft.setCursor(x, y);
  
  tft.println(display_monitor.digits_str);
}
void show_reference_value(void){
  update_colors();
  tft.loadFont(AA_FONT_SMALL);
  tft.setCursor(125, 100);
  tft.println("9999");
} 

void show_unit(void) {
  update_colors();
  tft.loadFont(AA_FONT_MEDIUM);
  tft.setCursor(170, 50);
  tft.println("U");
}


void show_battery(void) {
  update_colors();
  tft.loadFont(AA_FONT_SMALL);

  
  int w = tft.textWidth("BAT LOW"); 
  int h = tft.fontHeight();
  int y = 0;
  int x = TFT_WIDTH - w;
  
  //tft.drawRect(x, y, w, h, display_monitor.active_background_color);
  tft.setTextPadding(w+10);
  tft.setCursor(x, y);
  if(contents_monitor.battery == NORMAL_BATTERY)
  {
    tft.println("BAT OK");
  } 
  else if (contents_monitor.battery == LOW_BATTERY) 
  {
    tft.println("BAT LOW");
  }
} 

void show_diode(void){
  update_colors();
  tft.loadFont(AA_FONT_SMALL);
  tft.setCursor(125, 150);
  tft.println("D");
}

void show_impedances(void){

}


