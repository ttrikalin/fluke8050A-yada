#include "fluke_8050a_values.h"


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
  unsigned char tmp;

  switch(values_monitor.state) {

    case VALUES_MONITOR_STATE_INIT: {
      values_monitor.state = VALUES_MONITOR_STATE_WAIT;
      break;
    }

    case VALUES_MONITOR_STATE_WAIT: {
      if (values_monitor.read_flag) {
        values_monitor.read_flag = 0; 
        values_monitor.state = VALUES_MONITOR_STATE_READ;
      }
      break;
    }

    case VALUES_MONITOR_STATE_READ: {
      tmp = (unsigned char) ((digitalRead(fluke8050a_W)<<3)|
                             (digitalRead(fluke8050a_X)<<2)|
                             (digitalRead(fluke8050a_Y)<<1)|
                             (digitalRead(fluke8050a_Z)));
        
      switch(values_monitor.read_strobe){
        case ST0_0: {
          values_monitor.st0_value0 = tmp; 
          break;
        }
        case ST0_1: {
          values_monitor.st0_value1 = tmp; 
          break;
        }
        case ST0_2: {
          values_monitor.st0_value2 = tmp; 
          break;
        }
        case ST1: {
          values_monitor.st1_value = tmp; 
          break;
        }
        case ST2: {
          values_monitor.st2_value = tmp; 
          break;
        }
        case ST3: {
          values_monitor.st3_value = tmp; 
          break;
        }
        case ST4: {
          values_monitor.st4_value = tmp; 
          break;
        }
        default: {
          break;
        }
      }

      break;
    }

    default: {
      break;
    }

  }
}
