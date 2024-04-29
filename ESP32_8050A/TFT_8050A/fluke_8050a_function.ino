#include "TFT_8050A.h"

void function_monitor_initialize(void) {
  function_monitor.state = FUNCTION_MONITOR_STATE_INIT; 
  //function_monitor.read_flag = 0; 
  function_monitor.active_function = RELATIVE_RESISTANCE_0;
}

void function_monitor_tasks(void) {
  switch(function_monitor.state) {
    case FUNCTION_MONITOR_STATE_INIT: {
      function_monitor.state = FUNCTION_MONITOR_STATE_WAIT;
      break;
    }

    case FUNCTION_MONITOR_STATE_WAIT: {
      //if (function_monitor.read_flag) {
      if (!digits_monitor.in_strobe_phase) {
        function_monitor.state = FUNCTION_MONITOR_STATE_READ;
      }
      break;
    }

    case FUNCTION_MONITOR_STATE_READ: {
      
      function_monitor.active_function = (measurement_function)
        ((digitalRead(fluke8050a_FUNC_D)<<3)|
         (digitalRead(fluke8050a_FUNC_C)<<2)|
         (digitalRead(fluke8050a_FUNC_B)<<1)|
         (digitalRead(fluke8050a_FUNC_A)   )); 
      //function_monitor.read_flag = 0; 
      function_monitor.state = FUNCTION_MONITOR_STATE_WAIT;
      break;
    }

    default: {
      break;
    }
  }
}
