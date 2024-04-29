#include "TFT_8050A.h"

void range_monitor_initialize(void) {
  range_monitor.state = RANGE_MONITOR_STATE_INIT; 
  //range_monitor.read_flag = 0; 
  range_monitor.active_range = RANGE_20M;
}

void range_monitor_tasks(void) {
  switch(range_monitor.state) {
    case RANGE_MONITOR_STATE_INIT: {
      range_monitor.state = RANGE_MONITOR_STATE_WAIT;
      break;
    }

    case RANGE_MONITOR_STATE_WAIT: {
      //if (range_monitor.read_flag) {
      if (!digits_monitor.in_strobe_phase) {
        range_monitor.state = RANGE_MONITOR_STATE_READ;
      }
      break;
    }

    case RANGE_MONITOR_STATE_READ: {
      range_monitor.active_range = (measurement_range) 
        ((digitalRead(fluke8050a_RNG_C)<<2)|
         (digitalRead(fluke8050a_RNG_B)<<1)|
         (digitalRead(fluke8050a_RNG_A)   )); 
      //range_monitor.read_flag = 0; 
      range_monitor.state = RANGE_MONITOR_STATE_WAIT;
      break;
    }

    default: {
      break;
    }
  }
}
