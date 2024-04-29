#include "TFT_8050A.h"

void digits_monitor_initialize(void) {
  digits_monitor.state = DIGITS_MONITOR_STATE_INIT; 
  digits_monitor.read_flag = 0; 
  digits_monitor.in_strobe_phase = 0; 
  digits_monitor.st0_value0 = 0;
  digits_monitor.st0_value1 = 0; 
  digits_monitor.st0_value2 = 0;
  digits_monitor.st1_value  = 0;
  digits_monitor.st2_value  = 0;
  digits_monitor.st3_value  = 0;
  digits_monitor.st4_value  = 0;
  digits_monitor.read_strobe = NO_STROBE;
}

void digits_monitor_tasks(void) {
  unsigned int tmp;

  switch(digits_monitor.state) {

    case DIGITS_MONITOR_STATE_INIT: {
      digits_monitor.state = DIGITS_MONITOR_STATE_WAIT;
      break;
    }

    case DIGITS_MONITOR_STATE_WAIT: {
      if (digits_monitor.read_flag) {
        digits_monitor.read_flag = 0; 
        digits_monitor.state = DIGITS_MONITOR_STATE_READ;
      }
      break;
    }

    case DIGITS_MONITOR_STATE_READ: {
      tmp = (unsigned int)  ((digitalRead(fluke8050a_W)<<3)|
                             (digitalRead(fluke8050a_X)<<2)|
                             (digitalRead(fluke8050a_Y)<<1)|
                             (digitalRead(fluke8050a_Z)));
        
      switch(digits_monitor.read_strobe){
        case ST0_0: {
          digits_monitor.st0_value0 = tmp; 
          break;
        }
        case ST0_1: {
          digits_monitor.st0_value1 = tmp; 
          break;
        }
        case ST0_2: {
          digits_monitor.st0_value2 = tmp; 
          break;
        }
        case ST1: {
          digits_monitor.st1_value = tmp; 
          break;
        }
        case ST2: {
          digits_monitor.st2_value = tmp; 
          break;
        }
        case ST3: {
          digits_monitor.st3_value = tmp; 
          break;
        }
        case ST4: {
          digits_monitor.st4_value = tmp;
          digits_monitor.in_strobe_phase = 0;  
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
