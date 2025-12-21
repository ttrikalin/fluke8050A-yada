#include "TFT_8050A.h"

extern fastTasksMonitorData fast_tasks_monitor; 

void IRAM_ATTR strobe0_ISR(void) {
  fast_tasks_monitor.isr_read_flag = true; 
  fast_tasks_monitor.isr_in_strobe_phase = true; 
  switch(fast_tasks_monitor.isr_active_strobe) {
    case ST0_0:
      fast_tasks_monitor.isr_active_strobe = ST0_1;
      break;
    case ST0_1:
      fast_tasks_monitor.isr_active_strobe = ST0_2;
      break;
    default:
      fast_tasks_monitor.isr_active_strobe = ST0_0;
      break;
  }
}

void IRAM_ATTR strobe1_ISR(void) {
  fast_tasks_monitor.isr_read_flag = true; 
  fast_tasks_monitor.isr_active_strobe = ST1;
}

void IRAM_ATTR strobe2_ISR(void) {
  fast_tasks_monitor.isr_read_flag = true; 
  fast_tasks_monitor.isr_active_strobe = ST2;
}

void IRAM_ATTR strobe3_ISR(void) {
  fast_tasks_monitor.isr_read_flag = true; 
  fast_tasks_monitor.isr_active_strobe = ST3;
}

void IRAM_ATTR strobe4_ISR(void) {
  fast_tasks_monitor.isr_read_flag = true; 
  fast_tasks_monitor.isr_active_strobe = ST4;
}

