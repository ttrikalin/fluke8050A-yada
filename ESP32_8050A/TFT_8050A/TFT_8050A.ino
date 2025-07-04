/*
TFT_8050A.ino for an ESP32_WROOM32 devkit 

TFT display implementation for Fluke 8050A multimeter. 
TA Trikalinos 

Based in part on work by Michael Damkier, Hamburg, Germany (michael@vondervotteimittiss.de), who
provided the decoding of the strobe lines and the function and range truth tables for the Fluke 8050A
*/


#include "TFT_8050A.h"

TFT_eSPI tft = TFT_eSPI();   


TFT_eSprite canvas = TFT_eSprite(&tft);
//TFT_eSprite high_voltage = TFT_eSprite(&tft);
//TFT_eSprite battery = TFT_eSprite(&tft);
//TFT_eSprite diode = TFT_eSprite(&tft);
TFT_eSprite zone_0 = TFT_eSprite(&tft);
TFT_eSprite zone_1 = TFT_eSprite(&tft);
TFT_eSprite zone_2 = TFT_eSprite(&tft);
TFT_eSprite zone_3 = TFT_eSprite(&tft);


functionMonitorData function_monitor; 
rangeMonitorData range_monitor; 
digitsMonitorData digits_monitor; 
contentsMonitorData contents_monitor; 
displayMonitorData display_monitor; 

oneSymbol large_decimal_point; 
oneSymbol small_decimal_point; 


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
  range_monitor_initialize(); 
  function_monitor_initialize();
  digits_monitor_initialize();
  contents_monitor_initialize();
  display_monitor_initialize(); 
}

void loop(void) {
  delay(10); 
  digits_monitor_tasks(); 
  range_monitor_tasks();
  function_monitor_tasks();
  contents_monitor_tasks();
  display_monitor_tasks(); 
}

