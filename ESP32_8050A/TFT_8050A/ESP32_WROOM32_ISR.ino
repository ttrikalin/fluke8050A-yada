#include "TFT_8050A.h"


void IRAM_ATTR strobe0_ISR(void) {
  digits_monitor.read_flag = 1; 
  digits_monitor.in_strobe_phase = 1; 
  if (digits_monitor.read_strobe == ST0_0) {
    digits_monitor.read_strobe = ST0_1;  
  } else if (digits_monitor.read_strobe == ST0_1) {
    digits_monitor.read_strobe = ST0_2;
  } else {
    digits_monitor.read_strobe = ST0_0;
  }
}

void IRAM_ATTR strobe1_ISR(void) {
  digits_monitor.read_flag = 1; 
  digits_monitor.read_strobe = ST1;
}

void IRAM_ATTR strobe2_ISR(void) {
  digits_monitor.read_flag = 1; 
  digits_monitor.read_strobe = ST2;
}

void IRAM_ATTR strobe3_ISR(void) {
  digits_monitor.read_flag = 1; 
  digits_monitor.read_strobe = ST3;
}

void IRAM_ATTR strobe4_ISR(void) {
  digits_monitor.read_flag = 1; 
  digits_monitor.read_strobe = ST4;
}

