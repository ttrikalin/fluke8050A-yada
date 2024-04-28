#include "TFT8050A.h"


void initialize_ESP32_WROOM32(void) {
  Serial.begin(115200);  
  // 
  pinMode(BIT_8050_RNGc, INPUT);
  pinMode(BIT_8050_RNGb, INPUT);
  pinMode(BIT_8050_RNGa, INPUT);
  pinMode(BIT_8050_BT, INPUT);

  // func 
  pinMode(BIT_8050_Fb, INPUT);
  pinMode(BIT_8050_Fd, INPUT);
  pinMode(BIT_8050_Fa, INPUT);
  pinMode(BIT_8050_Fc, INPUT);

  // 8050A strobe lines
  pinMode(BIT_ST0, INPUT);
  attachInterrupt(BIT_ST0, strobe0_ISR, FALLING)
  pinMode(BIT_ST1, INPUT);
  attachInterrupt(BIT_ST1, strobe1_ISR, FALLING)
  pinMode(BIT_ST2, INPUT);
  attachInterrupt(BIT_ST2, strobe2_ISR, FALLING)
  pinMode(BIT_ST3, INPUT);
  attachInterrupt(BIT_ST3, strobe3_ISR, FALLING)
  pinMode(BIT_ST4, INPUT);
  attachInterrupt(BIT_ST4, strobe4_ISR, FALLING)

  // 8050A scancode lines
  pinMode(BIT_HV, INPUT);
  pinMode(BIT_DP, INPUT);
  pinMode(BIT_Z, INPUT);
  pinMode(BIT_Y, INPUT);
  pinMode(BIT_X, INPUT);
  pinMode(BIT_W, INPUT);

  pinMode(TFT_SCLK, OUTPUT);
  pinMode(TFT_MOSI, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_RST, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
}


void initialize_display(void) {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
}