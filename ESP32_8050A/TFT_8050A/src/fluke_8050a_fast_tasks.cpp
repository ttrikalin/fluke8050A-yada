#include "TFT_8050A.h"
 
extern FastTasksMonitorData fast_tasks_monitor;
//extern InputsMonitorData inputs_monitor;  

void fast_tasks_monitor_initialize(void) {
  //fast_tasks_monitor.state = FAST_TASKS_MONITOR_STATE_INIT; 
  fast_tasks_monitor.isr_active_strobe = NO_STROBE;  // set in the ISR's , reset in read_strobe()
  fast_tasks_monitor.st0_value0 = 0; 
  fast_tasks_monitor.st0_value1 = 0; 
  fast_tasks_monitor.st0_value2 = 0; 
  fast_tasks_monitor.st1_value = 0; 
  fast_tasks_monitor.st2_value = 0; 
  fast_tasks_monitor.st3_value = 0; 
  fast_tasks_monitor.st4_value = 0;
  fast_tasks_monitor.decimal_value = 0; 
  fast_tasks_monitor.voltage_level = NO_VOLTAGE_LEVEL; 
  fast_tasks_monitor.sign = Sign::NO_SIGN;
  fast_tasks_monitor.DP_flag0_0 = false;  
  fast_tasks_monitor.DP_flag0_1 = false;  
  fast_tasks_monitor.DP_flag0_2 = false;  
  fast_tasks_monitor.DP_flag1 = false;  
  fast_tasks_monitor.DP_flag2 = false;  
  fast_tasks_monitor.DP_flag3 = false;  
  fast_tasks_monitor.DP_flag4 = false;  
}


