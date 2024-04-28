#include "TFT_8050A.h"


void initialize_ESP32_WROOM32(void) {
  Serial.begin(115200);  
  // 
  pinMode(fluke8050a_RNG_C, INPUT);
  pinMode(fluke8050a_RNG_B, INPUT);
  pinMode(fluke8050a_RNG_A, INPUT);
  pinMode(fluke8050a_BT, INPUT);

  // func 
  pinMode(fluke8050a_FUNC_B, INPUT);
  pinMode(fluke8050a_FUNC_D, INPUT);
  pinMode(fluke8050a_FUNC_A, INPUT);
  pinMode(fluke8050a_FUNC_C, INPUT);

  // 8050A strobe lines, set as interrupts 
  pinMode(fluke8050a_ST0, INPUT);
  attachInterrupt(fluke8050a_ST0, strobe0_ISR, FALLING)
  pinMode(fluke8050a_ST1, INPUT);
  attachInterrupt(fluke8050a_ST1, strobe1_ISR, FALLING)
  pinMode(fluke8050a_ST2, INPUT);
  attachInterrupt(fluke8050a_ST2, strobe2_ISR, FALLING)
  pinMode(fluke8050a_ST3, INPUT);
  attachInterrupt(fluke8050a_ST3, strobe3_ISR, FALLING)
  pinMode(fluke8050a_ST4, INPUT);
  attachInterrupt(fluke8050a_ST4, strobe4_ISR, FALLING)

  // 8050A scancode lines
  pinMode(fluke8050a_HV, INPUT);
  pinMode(fluke8050a_DP, INPUT);
  pinMode(fluke8050a_Z, INPUT);
  pinMode(fluke8050a_Y, INPUT);
  pinMode(fluke8050a_X, INPUT);
  pinMode(fluke8050a_W, INPUT);

  // // These are correctly set by the library 
  // pinMode(TFT_SCLK, OUTPUT);
  // pinMode(TFT_MOSI, OUTPUT);
  // pinMode(TFT_DC, OUTPUT);
  // pinMode(TFT_RST, OUTPUT);
  // pinMode(TFT_CS, OUTPUT);
}


void initialize_display(void) {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
}