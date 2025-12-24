#include "TFT_8050A.h"
 
extern FastTasksMonitorData fast_tasks_monitor;
//extern InputsMonitorData inputs_monitor;  

void fast_tasks_monitor_initialize(void) {
  fast_tasks_monitor.state = FAST_TASKS_MONITOR_STATE_INIT; 
  fast_tasks_monitor.isr_active_strobe = NO_STROBE;  // set in the ISR's , reset in read_strobe()
  //fast_tasks_monitor.isr_read_flag = false;          // set in the ISR's , reset in read_strobe()
  //fast_tasks_monitor.isr_in_strobe_phase = false;    // set in the ISR's , reset in read_strobe()
  fast_tasks_monitor.st0_value0 = 0; 
  fast_tasks_monitor.st0_value1 = 0; 
  fast_tasks_monitor.st0_value2 = 0; 
  fast_tasks_monitor.st1_value = 0; 
  fast_tasks_monitor.st2_value = 0; 
  fast_tasks_monitor.st3_value = 0; 
  fast_tasks_monitor.st4_value = 0; 
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

void fast_tasks_monitor_tasks(void) { 
  // switch(fast_tasks_monitor.state) {
  //   case FAST_TASKS_MONITOR_STATE_INIT: {
  //     fast_tasks_monitor.state = FAST_TASKS_MONITOR_STATE_WAIT;
  //     break;
  //   }
  //   case FAST_TASKS_MONITOR_STATE_WAIT: {
  //     if (fast_tasks_monitor.isr_read_flag) {
  //       fast_tasks_monitor.state = FAST_TASKS_MONITOR_STATE_READ;


  //       // #ifdef DEBUG_ENABLE
  //       //   //digitalWrite(DEBUG_LED, (unsigned int) !digitalRead(DEBUG_LED));
  //       // #endif
  //     }
  //     break;
  //   }
  //   case FAST_TASKS_MONITOR_STATE_READ: {
  //     // Clear flag before reading to detect if another interrupt occurs
  //     fast_tasks_monitor.isr_read_flag = false; 
  //     fast_tasks_monitor.state = FAST_TASKS_MONITOR_STATE_WAIT;
  //     // #ifdef DEBUG_ENABLE
  //     //   digitalWrite(DEBUG_LED, LOW);
  //     // #endif
  //     //read_strobe(); 
  //     //fast_tasks_monitor.voltage_level = read_high_voltage(); 
  //     // Check if another interrupt occurred while we were reading
  //     // If so, immediately process it
  //     if (fast_tasks_monitor.isr_read_flag) {
  //       fast_tasks_monitor.state = FAST_TASKS_MONITOR_STATE_READ;
  //     }

      

  //     break;
  //   }
  //   default: 
  //     break; 
  // }
}

void read_strobe(void) {
  // unsigned int tmp; 

  // tmp = (unsigned int)  ((digitalRead(fluke8050a_W)<<3)|
  //                        (digitalRead(fluke8050a_X)<<2)|
  //                        (digitalRead(fluke8050a_Y)<<1)|
  //                        (digitalRead(fluke8050a_Z)));

  // switch(fast_tasks_monitor.isr_active_strobe){
  //   case ST0_0: {
  //     fast_tasks_monitor.st0_value0 = tmp; 
  //     infer_sign(); 
  //     break;
  //   }
  //   case ST0_1: {
  //     fast_tasks_monitor.st0_value1 = tmp; 
  //     break;
  //   }
  //   case ST0_2: {
  //     fast_tasks_monitor.st0_value2 = tmp; 
  //     break;
  //   }
  //   case ST1: {
  //     fast_tasks_monitor.st1_value = tmp; 
  //     break;
  //   }
  //   case ST2: {
  //     fast_tasks_monitor.st2_value = tmp; 
  //     break;
  //   }
  //   case ST3: {
  //     fast_tasks_monitor.st3_value = tmp; 
  //     break;
  //   }
  //   case ST4: {
  //     fast_tasks_monitor.st4_value = tmp;
  //     fast_tasks_monitor.isr_active_strobe = NO_STROBE; 
  //     break;
  //   }
  //   default: {
  //     break;
  //   }
  // }
}

voltage_levels read_high_voltage(void) {
  return digitalRead(fluke8050a_HV) ? HIGH_VOLTAGE : LOW_VOLTAGE; 
} 


void infer_sign(void) {
  // The sign is determined by the ST0 digit (0bWXYZ)
  // bits W==1 (vertical part of '+'), X==1 (minus part '-') or nothing
  // as per the logic below  
  fast_tasks_monitor.sign = Sign::NO_SIGN;
  if (test_bit(fast_tasks_monitor.st0_value0, 2)) {
    fast_tasks_monitor.sign = Sign::NEGATIVE_SIGN;
  } 
  if (test_bit(fast_tasks_monitor.st0_value0, 2) &&
      test_bit(fast_tasks_monitor.st0_value0, 3)) {
    fast_tasks_monitor.sign = Sign::POSITIVE_SIGN;
  } 
}