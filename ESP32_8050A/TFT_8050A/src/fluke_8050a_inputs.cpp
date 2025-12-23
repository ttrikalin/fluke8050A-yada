#include "TFT_8050A.h"



extern InputsMonitorData inputs_monitor; 
extern FastTasksMonitorData fast_tasks_monitor;
extern RawInputs raw_inputs;

void inputs_monitor_initialize(void) {
  inputs_monitor.state = INPUTS_MONITOR_STATE_INIT;
  inputs_monitor.function = Function::INVALID;
  inputs_monitor.range = Range::INVALID;
  inputs_monitor.base_unit = BaseUnit::INVALID;
  inputs_monitor.unit  = Unit::NO_UNIT;
  inputs_monitor.diode_style = DiodeStyle::NO_DIODE; 
  inputs_monitor.decimal_point_position = DecimalPointPosition::NO_DECIMAL_POINT; 
  inputs_monitor.battery = BatteryStyle::NO_BATTERY; 
  inputs_monitor.acdc_mode = Mode::NO_ACDC; 
  inputs_monitor.relative_measurement = RelativeMeasurementStyle::ABSOLUTE_MEASUREMENT; 
  //inputs_monitor.loop_counter = 0; 
  inputs_monitor.new_values_flag = true;
  inputs_monitor.valid_input_combination = false;
}

void inputs_monitor_tasks(void) {
  switch(inputs_monitor.state) {
    case INPUTS_MONITOR_STATE_INIT: {
      inputs_monitor.state = INPUTS_MONITOR_STATE_WAIT;
      break;
    }
    case INPUTS_MONITOR_STATE_WAIT: {
      if (fast_tasks_monitor.isr_active_strobe == NO_STROBE) {
        inputs_monitor.state = INPUTS_MONITOR_STATE_READ;
      } 
      break;
    }
    case INPUTS_MONITOR_STATE_READ: {
      read_raw_inputs(raw_inputs); 
      inputs_monitor.new_values_flag = raw_inputs.new_values_flag; 
      if (raw_inputs.new_values_flag) {
        decode_raw_inputs(raw_inputs, inputs_monitor);
      }
      inputs_monitor.state = INPUTS_MONITOR_STATE_WAIT;
      break;
    }
  }
}



DiodeStyle infer_diode_style(const Function function, const Range range) {
  DiodeStyle diode_style = DiodeStyle::NO_DIODE;
  if (function == Function::RESISTANCE && 
      range == Range::R_2) {
    diode_style = DiodeStyle::DIODE;
  } else if (function == Function::RESISTANCE && 
             (range == Range::R_20M ||
              range == Range::R_200 )) {
    diode_style = DiodeStyle::SMALL_DIODE;
  }
  return diode_style;
}





void read_raw_inputs(RawInputs &raw_inputs) {
  raw_inputs.new_values_flag = false; 
  unsigned int tmp;
  tmp = (unsigned int) ((digitalRead(fluke8050a_RNG_C)<<2)|
                        (digitalRead(fluke8050a_RNG_B)<<1)|
                        (digitalRead(fluke8050a_RNG_A)));
  if(raw_inputs.range_bits != tmp){
    raw_inputs.new_values_flag = true; 
    raw_inputs.range_bits = tmp;
  }
  
  // tmp = (unsigned int) ((digitalRead(fluke8050a_FUNC_A)<<3)|
  //                       (digitalRead(fluke8050a_FUNC_D)<<2)|
  //                       (digitalRead(fluke8050a_FUNC_C)<<1)|
  //                       (digitalRead(fluke8050a_FUNC_B)));
  tmp = (unsigned int) ((digitalRead(fluke8050a_FUNC_C)<<1)|
                        (digitalRead(fluke8050a_FUNC_B)));
  if(raw_inputs.function_bits != tmp){
    raw_inputs.new_values_flag = true; 
    raw_inputs.function_bits = tmp;
  }

  if (raw_inputs.AC_flag != digitalRead(fluke8050a_FUNC_A)){
    raw_inputs.AC_flag = !raw_inputs.AC_flag;
    raw_inputs.new_values_flag = true; 
  }

  if(raw_inputs.absolute_flag != digitalRead(fluke8050a_FUNC_D)){
    raw_inputs.absolute_flag = !raw_inputs.absolute_flag;
    raw_inputs.new_values_flag = true;
  }

  if (raw_inputs.battery_low_flag != digitalRead(fluke8050a_BT)){ 
    raw_inputs.battery_low_flag = !raw_inputs.battery_low_flag; 
    raw_inputs.new_values_flag = true; 
  } 
} 


Range infer_range(unsigned int range_bits) {
  switch(range_bits) {
    case 0:   return Range::R_20M;
    case 1:   return Range::R_200;
    case 2:   return Range::R_2mS;
    case 3:   return Range::R_2;
    case 4:   return Range::R_200nS;
    case 5:   return Range::R_20;
    case 6:   return Range::R_0P2;
    case 7:   return Range::R_2000;
    default:  return Range::INVALID;
  }
}

Function infer_function(unsigned int function_bits) {
  switch(function_bits) {
    case 0:   return Function::RESISTANCE;
    case 1:   return Function::VOLTAGE; 
    case 2:   return Function::CURRENT; 
    case 3:   return Function::GAIN;
    default:  return Function::INVALID;
  }
}

bool is_valid_input_combination(Function function, Range range){
  if((function == Function::VOLTAGE || function == Function::CURRENT) && range == Range::R_20M){
    return false;    
  }
  return(function != Function::INVALID && range != Range::INVALID);
}

DecimalPointPosition infer_decimal_point_position(const Function function, const Range range) {
  if(function == Function::GAIN && range == Range::R_20M){ 
    return DecimalPointPosition::NO_DECIMAL_POINT;
  }
  if(function == Function::GAIN && range != Range::R_20M){ 
    return DecimalPointPosition::DECIMAL_POINT_AT_TWO;
  }
  switch (range){
    case Range::R_2: {
      return DecimalPointPosition::DECIMAL_POINT_AT_ZERO;
    }
    case Range::R_20M:
    case Range::R_20: {
      return DecimalPointPosition::DECIMAL_POINT_AT_ONE;
    }
    case Range::R_0P2:
    case Range::R_200:
    case Range::R_200nS:
    case Range::R_2mS: {
      return DecimalPointPosition::DECIMAL_POINT_AT_TWO;
    }
    case Range::R_2000: {
      return DecimalPointPosition::DECIMAL_POINT_AT_THREE;
    }
    default: {
      return DecimalPointPosition::NO_DECIMAL_POINT;
    }
  }
}



void decode_raw_inputs(const RawInputs &raw_inputs, InputsMonitorData &imd) {
  imd.function = infer_function(raw_inputs.function_bits); 
  imd.range = infer_range(raw_inputs.range_bits); 
  imd.valid_input_combination = is_valid_input_combination(imd.function, imd.range);

  if(!imd.valid_input_combination){
    imd.base_unit = BaseUnit::INVALID; 
    imd.unit = Unit::NO_UNIT; 
    imd.decimal_point_position = DecimalPointPosition::NO_DECIMAL_POINT; 
    imd.acdc_mode = Mode::NO_ACDC; 
    return; 
  }

  // base unit and unit 
  switch(imd.function){
    case Function::VOLTAGE: {
      imd.base_unit = BaseUnit::VOLT; 
      imd.unit = (imd.range == Range::R_0P2) ? Unit::MILLI_VOLT : Unit::VOLT; 
      break;
    }
    case Function::CURRENT: {
      imd.base_unit = BaseUnit::AMPERE; 
      imd.unit = (imd.range == Range::R_0P2) ? Unit::MICRO_AMPERE : Unit::MILLI_AMPERE; 
      break;
    }
    case Function::RESISTANCE: {
      if(imd.range == Range::R_200nS || imd.range == Range::R_2mS){
        imd.base_unit = BaseUnit::SIEMENS; 
        imd.unit = (imd.range == Range::R_200nS) ? Unit::NANO_SIEMENS : Unit::MILLI_SIEMENS; 
        break;
      } 
      imd.base_unit = BaseUnit::OHM; 
      if(imd.range == Range::R_0P2){
        imd.unit = Unit::OHM; 
      } 
      else if(imd.range == Range::R_20M){
        imd.unit = Unit::MEGA_OHM; 
      } else {
        imd.unit = Unit::KILO_OHM; 
      }
      break;
    }
    case Function::GAIN: {
      if(imd.range == Range::R_20M){
        imd.base_unit = BaseUnit::IMPEDANCE_Z; 
        imd.unit = Unit::IMPEDANCE_Z; 
        break; 
      }
      imd.base_unit = BaseUnit::DECIBEL; 
      imd.unit = Unit::DECIBEL; 
      break; 
    }
    default: {
      imd.base_unit = BaseUnit::INVALID; 
      imd.unit = Unit::NO_UNIT; 
      break; 
    }
  }

  // diode style
  imd.diode_style = infer_diode_style(imd.function, imd.range);

  // decimal point position depends on both function and range
  imd.decimal_point_position = infer_decimal_point_position(imd.function, imd.range);

  // battery style depends on battery flag
  #ifdef fluke8050a_WITH_BATTERY
    imd.battery = raw_inputs.battery_low_flag ? BatteryStyle::LOW_BATTERY : BatteryStyle::NORMAL_BATTERY; 
  #else
    imd.battery = BatteryStyle::NO_BATTERY; 
  #endif

  // mode AC/DC depends on base unit
  if(imd.base_unit == BaseUnit::VOLT || imd.base_unit == BaseUnit::AMPERE){
    imd.acdc_mode = raw_inputs.AC_flag ? Mode::AC : Mode::DC; 
  } else {
    imd.acdc_mode = Mode::NO_ACDC; 
  }

  // relative measurement style depends on function bits
  imd.relative_measurement = raw_inputs.absolute_flag ? RelativeMeasurementStyle::ABSOLUTE_MEASUREMENT : RelativeMeasurementStyle::RELATIVE_MEASUREMENT;
}

