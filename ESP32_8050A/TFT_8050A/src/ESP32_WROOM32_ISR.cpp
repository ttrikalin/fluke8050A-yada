#include "TFT_8050A.h"

extern FastTasksMonitorData fast_tasks_monitor; 

void IRAM_ATTR strobe0_ISR(void) {
  #ifdef DEBUG_ENABLE
    digitalWrite(DEBUG_LED, HIGH);
  #endif
  
  fast_tasks_monitor.voltage_level = digitalRead(fluke8050a_HV) ? HIGH_VOLTAGE : LOW_VOLTAGE; 
  switch(fast_tasks_monitor.isr_active_strobe) {
    case ST0_0:
      fast_tasks_monitor.isr_active_strobe = ST0_1;
      fast_tasks_monitor.DP_flag0_0 = digitalRead(fluke8050a_DP); 
      fast_tasks_monitor.st0_value0 = (unsigned int)  ((digitalRead(fluke8050a_W)<<3)| (digitalRead(fluke8050a_X)<<2)|(digitalRead(fluke8050a_Y)<<1)|(digitalRead(fluke8050a_Z)));
      infer_sign();
      break;
    case ST0_1:
      fast_tasks_monitor.isr_active_strobe = ST0_2;
      fast_tasks_monitor.DP_flag0_1 = digitalRead(fluke8050a_DP); 
      fast_tasks_monitor.st0_value1 = (unsigned int)  ((digitalRead(fluke8050a_W)<<3)| (digitalRead(fluke8050a_X)<<2)|(digitalRead(fluke8050a_Y)<<1)|(digitalRead(fluke8050a_Z)));
      break;
    default:
      fast_tasks_monitor.isr_active_strobe = ST0_0;
      fast_tasks_monitor.DP_flag0_2 = digitalRead(fluke8050a_DP); 
      fast_tasks_monitor.st0_value2 = (unsigned int)  ((digitalRead(fluke8050a_W)<<3)| (digitalRead(fluke8050a_X)<<2)|(digitalRead(fluke8050a_Y)<<1)|(digitalRead(fluke8050a_Z)));
      break;
  }
  #ifdef DEBUG_ENABLE
    digitalWrite(DEBUG_LED, LOW);
  #endif
}

void IRAM_ATTR strobe1_ISR(void) {
  fast_tasks_monitor.isr_active_strobe = ST1;
  fast_tasks_monitor.DP_flag1 = digitalRead(fluke8050a_DP);  
  fast_tasks_monitor.st1_value =
    (unsigned int)  ((digitalRead(fluke8050a_W)<<3)|
                     (digitalRead(fluke8050a_X)<<2)|
                     (digitalRead(fluke8050a_Y)<<1)|
                     (digitalRead(fluke8050a_Z))); 
}

void IRAM_ATTR strobe2_ISR(void) {
  fast_tasks_monitor.isr_active_strobe = ST2;
  fast_tasks_monitor.DP_flag2 = digitalRead(fluke8050a_DP); 
  fast_tasks_monitor.st2_value =
    (unsigned int)  ((digitalRead(fluke8050a_W)<<3)|
                     (digitalRead(fluke8050a_X)<<2)|
                     (digitalRead(fluke8050a_Y)<<1)|
                     (digitalRead(fluke8050a_Z))); 
}

void IRAM_ATTR strobe3_ISR(void) {
  fast_tasks_monitor.isr_active_strobe = ST3;
  fast_tasks_monitor.DP_flag3 = digitalRead(fluke8050a_DP); 
  fast_tasks_monitor.st3_value =
    (unsigned int)  ((digitalRead(fluke8050a_W)<<3)|
                     (digitalRead(fluke8050a_X)<<2)|
                     (digitalRead(fluke8050a_Y)<<1)|
                     (digitalRead(fluke8050a_Z))); 
}

void IRAM_ATTR strobe4_ISR(void) {
  //fast_tasks_monitor.isr_read_flag = true; 
  if(!digitalRead(fluke8050a_ST4)) {
    fast_tasks_monitor.isr_active_strobe = NO_STROBE;   
    return;
  } 
  fast_tasks_monitor.isr_active_strobe = ST4;
  fast_tasks_monitor.DP_flag4 = digitalRead(fluke8050a_DP); 
  fast_tasks_monitor.st4_value =
    (unsigned int)  ((digitalRead(fluke8050a_W)<<3)|
                     (digitalRead(fluke8050a_X)<<2)|
                     (digitalRead(fluke8050a_Y)<<1)|
                     (digitalRead(fluke8050a_Z))); 
}


