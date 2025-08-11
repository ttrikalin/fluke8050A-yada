#include <TFT_eSPI.h>

/*
TFT_8050A.ino for an ESP32_WROOM32 devkit 

TFT display implementation for Fluke 8050A multimeter. 
TA Trikalinos 

Based in part on work by Michael Damkier, Hamburg, Germany (michael@vondervotteimittiss.de), who
provided the decoding of the strobe lines and the function and range truth tables for the Fluke 8050A
*/


#include "TFT_8050A.h"

TFT_eSPI tft = TFT_eSPI();   


TFT_eSprite measurements_sprite = TFT_eSprite(&tft);

displayMonitorData display_monitor; 
slowTasksMonitorData slow_tasks_monitor; 
fastTasksMonitorData fast_tasks_monitor; 

oneSymbol large_decimal_point; 
oneSymbol small_decimal_point; 
oneSymbol battery_full; 
oneSymbol battery_low; 
oneSymbol diode; 
oneSymbol high_voltage; 

arrayOfSymbols large_sign; 
arrayOfSymbols small_sign; 
arrayOfSymbols large_digit; 
arrayOfSymbols small_digit; 
arrayOfSymbols large_unit_symbol; 
arrayOfSymbols small_unit_symbol; 
arrayOfSymbols large_mode_symbol; 
arrayOfSymbols small_mode_symbol; 


// the setup and loop
void setup(void) {
  
  ESP32_WROOM32_initialize();
  slow_tasks_monitor_initialize(); 
  fast_tasks_monitor_initialize();
  display_monitor_initialize();
  
}

void loop(void) {
  //delay(LOOP_DELAY_MILLISECONDS); 
  slow_tasks_monitor_tasks(); 
  fast_tasks_monitor_tasks();
  display_monitor_tasks(); 
}

