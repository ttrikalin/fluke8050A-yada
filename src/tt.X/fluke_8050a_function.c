#include "fluke_8050a_function.h"


void function_monitor_initialize(void) {
  function_monitor.state = FUNCTION_MONITOR_STATE_INIT; 
  function_monitor.read_flag = 0; 
  function_monitor.active_function = RELATIVE_RESISTANCE;
}

void function_monitor_tasks(void) {
  switch(function_monitor.state) {
    case FUNCTION_MONITOR_STATE_INIT: {
      function_monitor.state = FUNCTION_MONITOR_STATE_WAIT;
      break;
    }

    case FUNCTION_MONITOR_STATE_WAIT: {
      if (function_monitor.read_flag) {
        function_monitor.state = FUNCTION_MONITOR_STATE_READ;
      }
      break;
    }

    case FUNCTION_MONITOR_STATE_READ: {
      
      function_monitor.active_function = 
        (unsigned char) 
        ((fluke8050a_FUNC_D<<3)|
         (fluke8050a_FUNC_C<<2)|
         (fluke8050a_FUNC_B<<1)|
         (fluke8050a_FUNC_A)); 
      function_monitor.read_flag = 0; 
      function_monitor.state = FUNCTION_MONITOR_STATE_WAIT;
      break;
    }

    default: {
      break;
    }

  }
}
