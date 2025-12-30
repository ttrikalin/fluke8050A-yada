#include "TFT_8050A.h"

const uint8_t *  digit_lg[10] PROGMEM = {
  digit_lg_0,
  digit_lg_1,
  digit_lg_2,
  digit_lg_3,
  digit_lg_4,
  digit_lg_5,
  digit_lg_6,
  digit_lg_7,
  digit_lg_8,
  digit_lg_9
};

const uint8_t *  sign_lg[2] PROGMEM = {
  sign_lg_plus,
  sign_lg_minus
};

const uint8_t * mode_sm[2] PROGMEM = {
  dc_symbol_sm,
  ac_symbol_sm
};

const uint8_t * unit_sm[11] PROGMEM = {
  unit_sm_0,
  unit_sm_1,
  unit_sm_2,
  unit_sm_3,
  unit_sm_4,
  unit_sm_5,
  unit_sm_6,
  unit_sm_7,
  unit_sm_8,
  unit_sm_9,
  unit_sm_10
};



const uint8_t * mode_lg[2] PROGMEM = {
  dc_symbol_lg,
  ac_symbol_lg
};

const uint8_t * unit_lg[11] PROGMEM = {
  unit_lg_0,
  unit_lg_1,  
  unit_lg_2,
  unit_lg_3,
  unit_lg_4,
  unit_lg_5,
  unit_lg_6,
  unit_lg_7,
  unit_lg_8,
  unit_lg_9,
  unit_lg_10
};

const uint8_t * sign_sm[2] PROGMEM = {
  sign_sm_plus,
  sign_sm_minus
};

const uint8_t *  digit_sm[10] PROGMEM = {
  digit_sm_0,
  digit_sm_1,
  digit_sm_2,
  digit_sm_3,
  digit_sm_4,
  digit_sm_5,
  digit_sm_6,
  digit_sm_7,
  digit_sm_8,
  digit_sm_9
};
