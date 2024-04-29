#include "TFT_8050A.h"

void display_monitor_initialize(void) {
  display_monitor.state = DISPLAY_MONITOR_STATE_INIT;

  display_monitor.active_background_color = TFT_BLACK;
  display_monitor.active_text_color = TFT_WHITE;

  display_monitor.relative_reference = 0.0;
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
      if(contents_monitor.high_voltage) {
        show_high_voltage();         
      } 
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
  tft.loadFont(AA_FONT_MEDIUM);
  tft.setCursor(50, 50);
  tft.println("SPLASH!");
  //tft.unloadFont();

  delay(TFT_SPLASH_SCREEN_DURATION);   
  tft.fillScreen(display_monitor.active_background_color);
} 

void format_digits(void){

}

void show_high_voltage(void){

}

void show_digits(void){
  update_colors();
  tft.setCursor(50, 90);
  tft.println("DIGITS!");
}
void show_reference_value(void){

} 

void show_unit(void) {

}


void show_battery(void) {

} 

void show_diode(void){

}

void show_impedances(void){

}


