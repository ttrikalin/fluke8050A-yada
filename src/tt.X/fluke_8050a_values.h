#ifndef _SRC_FLUKE_8050A_VALUES_H
#define	_SRC_FLUKE_8050A_VALUES_H


/* Application states */

typedef enum  {
    VALUES_MONITOR_STATE_INIT = 0, 
    VALUES_MONITOR_STATE_WAIT = 1, 
    VALUES_MONITOR_STATE_ST0  = 2,
    VALUES_MONITOR_STATE_ST1  = 3,
    VALUES_MONITOR_STATE_ST2  = 4,
    VALUES_MONITOR_STATE_ST3  = 5,
    VALUES_MONITOR_STATE_ST4  = 6
} values_monitor_states; 

typedef enum {
    NONE = 0, 
    ST0  = 1, 
    ST1  = 2, 
    ST2  = 3, 
    ST3  = 4, 
    ST4  = 5
} strobe_number; 

/* Application data */


typedef struct {
    values_monitor_states state; 
    strobe_number last_strobe,
    unsigned char read_flag; 
    unsigned char st0_value0,
    unsigned char st0_value1,
    unsigned char st0_value2, 
    unsigned char st1_value, 
    unsigned char st2_value, 
    unsigned char st3_value, 
    unsigned char st4_value, 
    unsigned char in_strobe_phase 
} valuesMonitorData; 

extern valuesMonitorData values_monitor; 

/* Function prototypes */ 

void values_monitor_initialize(void); 
void values_monitor_start_timer(void); 
void values_monitor_tasks(void); 

#endif	

