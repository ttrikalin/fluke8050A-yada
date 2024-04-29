#include "TFT_8050A.h"

void contents_monitor_initialize(void) {
  contents_monitor.state = CONTENTS_MONITOR_STATE_INIT; 
  contents_monitor.unit = NO_UNIT, 
  contents_monitor.quantity = NO_QUANTITY, 
  contents_monitor.diode_style = NO_DIODE;
  contents_monitor.decimal_point_position = NO_DECIMAL_POINT;
  contents_monitor.sign = NO_SIGN;
  contents_monitor.alternating_current = 0;
  contents_monitor.high_voltage = 0; 
  if(fluke8050a_WITH_BATTERY) {
    contents_monitor.battery = NORMAL_BATTERY;   
  } else {
    contents_monitor.battery = NO_BATTERY;   
  }
  contents_monitor.relative_measurement = 0;
}

void contents_monitor_tasks(void) {
  switch(contents_monitor.state) {
    case CONTENTS_MONITOR_STATE_INIT: {
      contents_monitor.state = CONTENTS_MONITOR_STATE_WAIT;
      break;
    }

    case CONTENTS_MONITOR_STATE_WAIT: {
      if (!digits_monitor.in_strobe_phase) {
        contents_monitor.state = CONTENTS_MONITOR_STATE_INFER;
      }
      break;
    }

    case CONTENTS_MONITOR_STATE_INFER: {
      infer_alternating_current(); 
      infer_relative_measurement(); 
      infer_high_voltage(); 
      infer_low_battery(); 
      infer_quantity(); 
      infer_unit(); 
      infer_diode_style();       
      infer_decimal_point_position(); 
      infer_sign(); 
      break;
    }

    default: {
      break;
    }
  }
}



void infer_alternating_current(void) {
  // fluke8050a_FUNC_A = 0 vs 1 <==> DC vs AC 
  contents_monitor.alternating_current = 
    test_bit(function_monitor.active_function, 0); 
} 

void infer_relative_measurement(void) {
  // fluke8050a_FUNC_D = 0 vs 1 <==> REL vs NOREL 
  contents_monitor.relative_measurement = 
    !test_bit(function_monitor.active_function, 3); 
} 

void infer_high_voltage(void) {
  contents_monitor.high_voltage = digitalRead(fluke8050a_HV);
} 

void infer_low_battery(void) {
  // fluke8050a_BT is set when battery is low. 
  switch(contents_monitor.battery) {
    case NORMAL_BATTERY:  
    case LOW_BATTERY: 
      // this longer code is fine because it allows 
      // changing the enum level order 
      if(digitalRead(fluke8050a_BT)) {
        contents_monitor.battery = LOW_BATTERY;
      } else {
        contents_monitor.battery = NORMAL_BATTERY;
      }
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
  switch(function_monitor.state){
    case RELATIVE_RESISTANCE_0:
    case RELATIVE_RESISTANCE_1:
    case ABSOLUTE_RESISTANCE_0:
    case ABSOLUTE_RESISTANCE_1: {
      if (range_monitor.state == RANGE_200nS || 
          range_monitor.state == RANGE_2mS) {
        contents_monitor.quantity = CONDUCTANCE; 
        break;
      } else {
        contents_monitor.quantity = RESISTANCE; 
        break;
      }
    }
    case RELATIVE_DC_VOLTAGE: 
    case RELATIVE_AC_VOLTAGE:
    case ABSOLUTE_DC_VOLTAGE:
    case ABSOLUTE_AC_VOLTAGE: {
      contents_monitor.quantity = VOLTAGE; 
      break;
    }
    case RELATIVE_DC_CURRENT: 
    case RELATIVE_AC_CURRENT:
    case ABSOLUTE_DC_CURRENT:
    case ABSOLUTE_AC_CURRENT: {
      contents_monitor.quantity = CURRENT; 
      break;
    }
    case GAIN_DB_0:
    case GAIN_DB_1:
    case GAIN_DB_2:
    case GAIN_DB_3: {
      contents_monitor.quantity = GAIN; 
      break; 
    }

    default: {
      contents_monitor.quantity = NO_QUANTITY;
      break; 
    }
  } 
}

void infer_unit(void) {
  switch(contents_monitor.quantity){

    case CONDUCTANCE: {
      if (range_monitor.active_range == RANGE_200nS) {
        contents_monitor.unit = NANO_SIEMENS; 
      } else {
        contents_monitor.unit = MILLI_SIEMENS; 
      }
      break;
    }

    case RESISTANCE: {
      if (range_monitor.active_range == RANGE_20M) {
        contents_monitor.unit = MEGA_OHM; 
      } else if (range_monitor.active_range == RANGE_POINT_2) {
        contents_monitor.unit = OHM; 
      } else {
        contents_monitor.unit = KILO_OHM; 
      }
      break;
    }

    case VOLTAGE: {
      if (range_monitor.active_range == RANGE_POINT_2) {
        contents_monitor.unit = MILLI_VOLT; 
      } else {
        contents_monitor.unit = VOLT; 
      }
      break;
    }

    case CURRENT: {
      if (range_monitor.active_range == RANGE_POINT_2) {
        contents_monitor.unit = MICRO_AMPERE; 
      } else {
        contents_monitor.unit = MILLI_AMPERE; 
      }
      break;
    }

    case GAIN: {
      contents_monitor.unit = DECIBEL; 
      break;
    }

    default: {
      contents_monitor.unit = NO_UNIT;
      break;
    }
  }
}


void infer_diode_style(void) {
  contents_monitor.diode_style = NO_DIODE;
  if (contents_monitor.quantity == RESISTANCE && 
      range_monitor.active_range == RANGE_2) {
    contents_monitor.diode_style = DIODE;
  } else if (contents_monitor.quantity == RESISTANCE && 
             (range_monitor.active_range == RANGE_20M ||
              range_monitor.active_range == RANGE_200 )) {
    contents_monitor.diode_style = SMALL_DIODE;
  }
}


void infer_decimal_point_position(void) {
  if (contents_monitor.quantity == GAIN) {
    if (range_monitor.active_range == RANGE_20M) {
      contents_monitor.decimal_point_position = NO_DECIMAL_POINT;
    } else {
      contents_monitor.decimal_point_position = DECIMAL_POINT_AT_TWO;
    }
  } else {
    switch(range_monitor.active_range) {
      //case RANGE_2mS:
      case RANGE_2:{
        contents_monitor.decimal_point_position = DECIMAL_POINT_AT_ZERO;
        break;
      }

      case RANGE_20M:
      case RANGE_20: {
        contents_monitor.decimal_point_position = DECIMAL_POINT_AT_ONE;
        break;
      }

      case RANGE_POINT_2:
      case RANGE_200:
      case RANGE_200nS:
      case RANGE_2mS: {
        contents_monitor.decimal_point_position = DECIMAL_POINT_AT_TWO;
        break;
      }

      case RANGE_2000: {
        contents_monitor.decimal_point_position = DECIMAL_POINT_AT_THREE;
        break;
      }

      default: {
        contents_monitor.decimal_point_position = NO_DECIMAL_POINT;
        break;
      }
    }
  }
}


void infer_sign(void) {
  // The sign is determined by the ST0 digit (0bWXYZ)
  // bits W==1 (vertical part of '+'), X==1 (minus part '-') or nothing
  // as per the logic below  
  if ( test_bit(digits_monitor.st0_value0, 3) ) {
    contents_monitor.sign = POSITIVE_SIGN;
  } else if ( test_bit(digits_monitor.st0_value0, 2) ) {
    contents_monitor.sign = NEGATIVE_SIGN;
  } else {
    contents_monitor.sign = NO_SIGN;
  }
}
