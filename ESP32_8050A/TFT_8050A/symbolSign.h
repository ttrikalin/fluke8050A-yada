#ifndef SYMBOLSIGN_H
#define SYMBOLSIGN_H

// bmp width & height
#define W_SIGN_LG 32
#define H_SIGN_LG 32
#define W_SIGN_SM 12
#define H_SIGN_SM 12
// y offset from Y_DIGIT_LG
#define OFFSET_SIGN_LG 35
// y offset from Y_DIGIT_SM
#define OFFSET_SIGN_SM 16


#define SIGN_NONE  0
#define SIGN_PLUS  1
#define SIGN_MINUS 2


#define SIGN_LG(n) (n?(uint8_t*)pgm_read_word(&(symbolSign_lg[n-1])):SIGN_NONE)
#define SIGN_SM(n) (n?(uint8_t*)pgm_read_word(&(symbolSign_sm[n-1])):SIGN_NONE)

  // plus
const uint8_t symbolSign_lg_0[] PROGMEM =
  { 0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x0F, 0xE0, 0x00,
    0x00, 0x00, 0x00, 0x00 };
  // minus
const uint8_t symbolSign_lg_1[] PROGMEM =
  { 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0xFF, 0xFF, 0xFF, 0xFE,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00 };

const uint8_t * const symbolSign_lg[2] PROGMEM = {
  symbolSign_lg_0,
  symbolSign_lg_1
};

  // plus
const uint8_t symbolSign_sm_0[] PROGMEM =
  { 0x0E, 0x00,
    0x0E, 0x00,
    0x0E, 0x00,
    0x0E, 0x00,
    0xFF, 0xE0,
    0xFF, 0xE0,
    0xFF, 0xE0,
    0x0E, 0x00,
    0x0E, 0x00,
    0x0E, 0x00,
    0x0E, 0x00,
    0x00, 0x00 };
  // minus
const uint8_t symbolSign_sm_1[] PROGMEM =
  { 0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0xFF, 0xE0,
    0xFF, 0xE0,
    0xFF, 0xE0,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00 };

const uint8_t * const symbolSign_sm[2] PROGMEM = {
  symbolSign_sm_0,
  symbolSign_sm_1
};

#endif // SYMBOLSIGN_H