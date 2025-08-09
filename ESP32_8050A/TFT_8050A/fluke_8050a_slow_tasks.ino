#include "TFT_8050A.h"

void slow_tasks_monitor_initialize(void) {
  slow_tasks_monitor.state = SLOW_TASKS_MONITOR_STATE_INIT; 
  slow_tasks_monitor.unit = NO_UNIT; 
  slow_tasks_monitor.quantity = NO_QUANTITY; 
  slow_tasks_monitor.diode_style = NO_DIODE; 
  slow_tasks_monitor.decimal_point_position = NO_DECIMAL_POINT; 
  slow_tasks_monitor.battery = NO_BATTERY; 
  slow_tasks_monitor.acdc_mode = NO_ACDC; 
  slow_tasks_monitor.relative_measurement = ABSOLUTE_MEASUREMENT; 
  slow_tasks_monitor.active_function = RESISTANCE; 
  slow_tasks_monitor.active_range = RANGE_20M; 
  slow_tasks_monitor.loop_counter = UPDATE_SLOW_TASKS_EVERY_N_LOOPS; 
}

void slow_tasks_monitor_tasks(void) {
  switch(slow_tasks_monitor.state) {
    case SLOW_TASKS_MONITOR_STATE_INIT: {
      slow_tasks_monitor.state = SLOW_TASKS_MONITOR_STATE_WAIT;
      slow_tasks_monitor.loop_counter = UPDATE_SLOW_TASKS_EVERY_N_LOOPS; 
      break;
    }
    case SLOW_TASKS_MONITOR_STATE_WAIT: {
      slow_tasks_monitor.loop_counter++; 
      if (slow_tasks_monitor.loop_counter == UPDATE_SLOW_TASKS_EVERY_N_LOOPS) {
        slow_tasks_monitor.state = SLOW_TASKS_MONITOR_STATE_READ;
      }
      break;
    }
    case SLOW_TASKS_MONITOR_STATE_READ: {
      slow_tasks_monitor.loop_counter = 0; 
      read_active_function(); 
      read_active_range(); 
      infer_unit(); 
      infer_quantity(); 
      infer_diode_style(); 
      infer_decimal_point_position(); 
      infer_low_battery(); 
      infer_relative_measurement(); 
      infer_alternating_current(); 
      slow_tasks_monitor.state = SLOW_TASKS_MONITOR_STATE_WAIT;
      break;
    }
  }
}




void infer_alternating_current(void) {
  // fluke8050a_FUNC_A = 0 vs 1 <==> DC vs AC 
  slow_tasks_monitor.acdc_mode = 
    test_bit(slow_tasks_monitor.active_function, 0) ? AC : DC; 
} 

void infer_relative_measurement(void) {
  // fluke8050a_FUNC_D = 0 vs 1 <==> REL vs NOREL 
  slow_tasks_monitor.relative_measurement = 
    test_bit(slow_tasks_monitor.active_function, 3) ? 
      ABSOLUTE_MEASUREMENT : RELATIVE_MEASUREMENT; 
} 

// void infer_high_voltage(void) {
//   contents_monitor.voltage_level = digitalRead(fluke8050a_HV) ? 
//     HIGH_VOLTAGE : LOW_VOLTAGE; 
// } 

void infer_low_battery(void) {
  // fluke8050a_BT is set when battery is low. 
  switch(slow_tasks_monitor.battery) {
    case NORMAL_BATTERY:  
    case LOW_BATTERY: 
      slow_tasks_monitor.battery = digitalRead(fluke8050a_BT) ? 
        LOW_BATTERY : NORMAL_BATTERY; 
      break;
    
    case NO_BATTERY:
    default: 
      break; 
  }
} 

void infer_quantity(void) {
  // This can be made easier by checking only 
  // fluke8050a_FUNC_B, fluke8050a_FUNC_C
  // (bits 1,2 in function_monitor.state)
  // but we have plenty of space and computing power 
  switch(slow_tasks_monitor.active_function){
    case RELATIVE_RESISTANCE_0:
    case RELATIVE_RESISTANCE_1:
    case ABSOLUTE_RESISTANCE_0:
    case ABSOLUTE_RESISTANCE_1: {
      if (slow_tasks_monitor.active_range == RANGE_200nS || 
          slow_tasks_monitor.active_range == RANGE_2mS) {
        slow_tasks_monitor.quantity = CONDUCTANCE; 
        break;
      } else {
        slow_tasks_monitor.quantity = RESISTANCE; 
        break;
      }
    }
    case RELATIVE_DC_VOLTAGE: 
    case RELATIVE_AC_VOLTAGE:
    case ABSOLUTE_DC_VOLTAGE:
    case ABSOLUTE_AC_VOLTAGE: {
      slow_tasks_monitor.quantity = VOLTAGE; 
      break;
    }
    case RELATIVE_DC_CURRENT: 
    case RELATIVE_AC_CURRENT:
    case ABSOLUTE_DC_CURRENT:
    case ABSOLUTE_AC_CURRENT: {
      slow_tasks_monitor.quantity = CURRENT; 
      break;
    }
    case GAIN_DB_0:
    case GAIN_DB_1:
    case GAIN_DB_2:
    case GAIN_DB_3: {
      slow_tasks_monitor.quantity = GAIN; 
      break; 
    }

    default: {
      slow_tasks_monitor.quantity = NO_QUANTITY;
      break; 
    }
  } 
}

void infer_unit(void) {
  switch(slow_tasks_monitor.quantity){

    case CONDUCTANCE: {
      if (slow_tasks_monitor.active_range == RANGE_200nS) {
        slow_tasks_monitor.unit = NANO_SIEMENS; 
      } else {
        slow_tasks_monitor.unit = MILLI_SIEMENS; 
      }
      break;
    }

    case RESISTANCE: {
      if (slow_tasks_monitor.active_range == RANGE_20M) {
        slow_tasks_monitor.unit = MEGA_OHM; 
      } else if (slow_tasks_monitor.active_range == RANGE_POINT_2) {
        slow_tasks_monitor.unit = OHM; 
      } else {
        slow_tasks_monitor.unit = KILO_OHM; 
      }
      break;
    }

    case VOLTAGE: {
      if (slow_tasks_monitor.active_range == RANGE_POINT_2) {
        slow_tasks_monitor.unit = MILLI_VOLT; 
      } else {
        slow_tasks_monitor.unit = VOLT; 
      }
      break;
    }

    case CURRENT: {
      if (slow_tasks_monitor.active_range == RANGE_POINT_2) {
        slow_tasks_monitor.unit = MICRO_AMPERE; 
      } else {
        slow_tasks_monitor.unit = MILLI_AMPERE; 
      }
      break;
    }

    case GAIN: {
      slow_tasks_monitor.unit = DECIBEL; 
      break;
    }

    default: {
      slow_tasks_monitor.unit = NO_UNIT;
      break;
    }
  }
}


void infer_diode_style(void) {
  slow_tasks_monitor.diode_style = NO_DIODE;
  if (slow_tasks_monitor.quantity == RESISTANCE && 
      slow_tasks_monitor.active_range == RANGE_2) {
    slow_tasks_monitor.diode_style = DIODE;
  } else if (slow_tasks_monitor.quantity == RESISTANCE && 
             (slow_tasks_monitor.active_range == RANGE_20M ||
              slow_tasks_monitor.active_range == RANGE_200 )) {
    slow_tasks_monitor.diode_style = SMALL_DIODE;
  }
}


void infer_decimal_point_position(void) {
  if (slow_tasks_monitor.quantity == GAIN) {
    if (slow_tasks_monitor.active_range == RANGE_20M) {
      slow_tasks_monitor.decimal_point_position = NO_DECIMAL_POINT;
    } else {
      slow_tasks_monitor.decimal_point_position = DECIMAL_POINT_AT_TWO;
    }
  } else {
    switch(slow_tasks_monitor.active_range) {
      //case RANGE_2mS:
      case RANGE_2:{
        slow_tasks_monitor.decimal_point_position = DECIMAL_POINT_AT_ZERO;
        break;
      }

      case RANGE_20M:
      case RANGE_20: {
        slow_tasks_monitor.decimal_point_position = DECIMAL_POINT_AT_ONE;
        break;
      }

      case RANGE_POINT_2:
      case RANGE_200:
      case RANGE_200nS:
      case RANGE_2mS: {
        slow_tasks_monitor.decimal_point_position = DECIMAL_POINT_AT_TWO;
        break;
      }

      case RANGE_2000: {
        slow_tasks_monitor.decimal_point_position = DECIMAL_POINT_AT_THREE;
        break;
      }

      default: {
        slow_tasks_monitor.decimal_point_position = NO_DECIMAL_POINT;
        break;
      }
    }
  }
}



read_active_range(void) {
  slow_tasks_monitor.active_range = 
    (measurement_range) 
      ((digitalRead(fluke8050a_RNG_C)<<2)|
       (digitalRead(fluke8050a_RNG_B)<<1)|
       (digitalRead(fluke8050a_RNG_A)   )); 
}

void read_active_function(void) {
  slow_tasks_monitor.active_function = 
    (measurement_function)
      ((digitalRead(fluke8050a_FUNC_D)<<3)|
       (digitalRead(fluke8050a_FUNC_C)<<2)|
       (digitalRead(fluke8050a_FUNC_B)<<1)|
       (digitalRead(fluke8050a_FUNC_A)   )); 
}


// void infer_sign(void) {
//   // The sign is determined by the ST0 digit (0bWXYZ)
//   // bits W==1 (vertical part of '+'), X==1 (minus part '-') or nothing
//   // as per the logic below  
//   if ( test_bit(digits_monitor.st0_value0, 3) ) {
//     contents_monitor.sign = POSITIVE_SIGN;
//   } else if ( test_bit(digits_monitor.st0_value0, 2) ) {
//     contents_monitor.sign = NEGATIVE_SIGN;
//   } else {
//     contents_monitor.sign = NO_SIGN;
//   }
// }
