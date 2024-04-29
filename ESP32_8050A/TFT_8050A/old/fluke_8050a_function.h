#ifndef _SRC_FLUKE_8050A_FUNCTION_H
#define	_SRC_FLUKE_8050A_FUNCTION_H


/* Application states */

typedef enum  {
    FUNCTION_MONITOR_STATE_INIT = 0, 
    FUNCTION_MONITOR_STATE_WAIT = 1, 
    FUNCTION_MONITOR_STATE_READ = 2
} function_monitor_states; 


/* Application data */
typedef enum {
    RELATIVE_RESISTANCE_0 =  0,     // 0b0000
    RELATIVE_RESISTANCE_1 =  1,     // 0b0001
    RELATIVE_DC_VOLTAGE   =  2,     // 0b0010 
    RELATIVE_AC_VOLTAGE   =  3,     // 0b0011
    RELATIVE_DC_CURRENT   =  4,     // 0b0100 
    RELATIVE_AC_CURRENT   =  5,     // 0b0101   
    VOLTAGE_GAIN_DB_0     =  6,     // 0b0110
    VOLTAGE_GAIN_DB_1     =  7,     // 0b0111
    ABSOLUTE_RESISTANCE_0 =  8,     // 0b1000
    ABSOLUTE_RESISTANCE_1 =  9,     // 0b1001
    ABSOLUTE_DC_VOLTAGE   = 10,     // 0b1010
    ABSOLUTE_AC_VOLTAGE   = 11,     // 0b1011
    ABSOLUTE_DC_CURRENT   = 12,     // 0b1100
    ABSOLUTE_AC_CURRENT   = 13,     // 0b1101
    VOLTAGE_GAIN_DB_2     = 14,     // 0b1110
    VOLTAGE_GAIN_DB_3     = 15      // 0b1111
} measurement_function; 

typedef struct {
    function_monitor_states state; 
    unsigned char read_flag; 
    measurement_function active_function; 
} functionMonitorData; 

extern functionMonitorData function_monitor; 

/* Function prototypes */ 

void function_monitor_initialize(void); 
void function_monitor_tasks(void); 



#endif // _SRC_FLUKE_8050A_FUNCTION_H

