#ifndef _SRC_FLUKE_8050A_VALUES_H
#define	_SRC_FLUKE_8050A_VALUES_H


/* Application states */

typedef enum  {
    VALUES_MONITOR_STATE_INIT  = 0, 
    VALUES_MONITOR_STATE_WAIT  = 1, 
    VALUES_MONITOR_STATE_READ  = 2
} values_monitor_states; 

typedef enum {
    NONE   = 0, 
    ST0_0  = 1, 
    ST0_1  = 2, 
    ST0_2  = 3, 
    ST1    = 4, 
    ST2    = 5, 
    ST3    = 6, 
    ST4    = 7
} strobe_number; 

/* Application data */


typedef struct {
    values_monitor_states state; 
    strobe_number read_strobe;
    unsigned char read_flag; 
    unsigned char st0_value0;
    unsigned char st0_value1;
    unsigned char st0_value2; 
    unsigned char st1_value;
    unsigned char st2_value; 
    unsigned char st3_value; 
    unsigned char st4_value; 
} valuesMonitorData; 

extern valuesMonitorData values_monitor; 

/* Function prototypes */ 

void values_monitor_initialize(void); 
void values_monitor_tasks(void); 



#endif  // _SRC_FLUKE_8050A_VALUES_H

