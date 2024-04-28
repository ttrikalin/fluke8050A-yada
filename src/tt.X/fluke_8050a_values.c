#include "fluke_8050a_values.h"

void values_monitor_start_strobe_timer(void) {
  // code to start timer here 
  // Start a timer interrupt countdown that is long enough 
  // to go past 5+2 pulses
}

void values_monitor_initialize(void) {
  values_monitor.state = RANGE_MONITOR_STATE_INIT; 
  values_monitor.read_flag = 0; 
  values_monitor.st0_value0 = 0x00;
  values_monitor.st0_value1 = 0x00; 
  values_monitor.st0_value2 = 0x00;
  values_monitor.st1_value  = 0x00;
  values_monitor.st2_value  = 0x00;
  values_monitor.st3_value  = 0x00;
  values_monitor.st4_value  = 0x00;
  values_monitor.in_strobe_phase = 0; 
  values_monitor.last_strobe = NONE;
}

void values_monitor_tasks(void) {
  switch(values_monitor.state) {
    case VALUES_MONITOR_STATE_INIT: {
      values_monitor.state = VALUES_MONITOR_STATE_WAIT;
      break;
    }

    case VALUES_MONITOR_STATE_WAIT: {
      break;
    }

    case VALUES_MONITOR_STATE_ST0: {
      values_monitor.read_flag = 0; 
      if(values_monitor.last_strobe == ST5 || 
         values_monitor.last_strobe == NONE) {
        values_monitor.st0_value0 = 
          (unsigned char) ((fluke8050a_W<<3)|
                           (fluke8050a_X<<2)|
                           (fluke8050a_Y<<1)|
                           (fluke8050a_Z));
        values_monitor.st0_value1 = 0x00;
        values_monitor.st0_value2 = 0x00; 
      }

      break;
    }



    default: {
      break;
    }

  }
}
