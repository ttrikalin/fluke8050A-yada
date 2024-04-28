#ifndef _SRC_FLUKE_8050A_RANGE_H
#define	_SRC_FLUKE_8050A_RANGE_H


/* Application states */

typedef enum  {
    RANGE_MONITOR_STATE_INIT = 0, 
    RANGE_MONITOR_STATE_WAIT = 1, 
    RANGE_MONITOR_STATE_READ = 2
} range_monitor_states; 


/* Application data */
typedef enum {
    R_20_MOhm        =  0,     // 0b0000
    R_200_nS         =  1,     // 0b0001
    R_2_ms           =  2,     // 0b0010 
    R_200_Ohm_uA_mV  =  3,     // 0b0011
    R_200_KOhm_mA_V  =  4,     // 0b0100 
    R_20_KOhm_mA_V   =  5,     // 0b0101   
    R_2_KOhm_mA_V    =  6,     // 0b0110
    R_2000_KOhm_mA_V =  7      // 0b0111
} measurement_range; 

typedef struct {
    range_monitor_states state; 
    unsigned char read_flag; 
    measurement_range active_range; 
} rangeMonitorData; 

extern rangeMonitorData range_monitor; 

/* Function prototypes */ 

void range_monitor_initialize(void); 
void range_monitor_tasks(void); 

#endif	

