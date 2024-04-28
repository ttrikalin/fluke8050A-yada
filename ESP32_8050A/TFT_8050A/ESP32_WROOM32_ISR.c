#include #include "TFT8050A.h"


void IRAM_ATTR strobe0_ISR(void) {
  values_monitor.read_flag = 1; 
  if (values_monitor.read_strobe == ST0_0) {
    values_monitor.read_strobe = ST0_1;  
  } else if (values_monitor.read_strobe == ST0_1) {
    values_monitor.read_strobe = ST0_2;
  } else {
    values_monitor.read_strobe = ST0_0;
  }
}

void IRAM_ATTR strobe1_ISR(void) {
  values_monitor.read_flag = 1; 
  values_monitor.read_strobe = ST1;
}

void IRAM_ATTR strobe2_ISR(void) {
  values_monitor.read_flag = 1; 
  values_monitor.read_strobe = ST2;
}

void IRAM_ATTR strobe3_ISR(void) {
  values_monitor.read_flag = 1; 
  values_monitor.read_strobe = ST3;
}

void IRAM_ATTR strobe4_ISR(void) {
  values_monitor.read_flag = 1; 
  values_monitor.read_strobe = ST4;
}

