#include "TFT_8050A.h"

void ESP32_WROOM32_initialize(void) {
  Serial.begin(115200);  
  // 
  pinMode(fluke8050a_RNG_C, INPUT);
  pinMode(fluke8050a_RNG_B, INPUT);
  pinMode(fluke8050a_RNG_A, INPUT);
  pinMode(fluke8050a_BT, INPUT);

  // func 
  pinMode(fluke8050a_FUNC_A, INPUT);    // DC LOW
  pinMode(fluke8050a_FUNC_B, INPUT);    
  pinMode(fluke8050a_FUNC_C, INPUT);
  pinMode(fluke8050a_FUNC_D, INPUT);    // relative LOW
  
  // 8050A strobe lines, set as interrupts 
  pinMode(fluke8050a_ST0, INPUT);
  attachInterrupt(digitalPinToInterrupt(fluke8050a_ST0), strobe0_ISR, RISING);
  pinMode(fluke8050a_ST1, INPUT);
  attachInterrupt(digitalPinToInterrupt(fluke8050a_ST1), strobe1_ISR, RISING);
  pinMode(fluke8050a_ST2, INPUT);
  attachInterrupt(digitalPinToInterrupt(fluke8050a_ST2), strobe2_ISR, RISING);
  pinMode(fluke8050a_ST3, INPUT);
  attachInterrupt(digitalPinToInterrupt(fluke8050a_ST3), strobe3_ISR, RISING);
  pinMode(fluke8050a_ST4, INPUT);
  attachInterrupt(digitalPinToInterrupt(fluke8050a_ST4), strobe4_ISR, CHANGE);

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

  #ifdef DEBUG_ENABLE
    pinMode(DEBUG_LED, OUTPUT);
  #endif
}
