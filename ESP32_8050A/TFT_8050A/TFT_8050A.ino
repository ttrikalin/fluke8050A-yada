/*
TFT_8050A.ino

TFT display implementation for Fluke 8050A multimeter

Thank you to Adafruit for the following hardware and supporting software
Adafruit #296 Atmega32u4 Breakout Board
  http://www.adafruit.com/products/296
Adafruit #1480 2.2" TFT LCD display - ILI9340
  http://www.adafruit.com/products/1480

Michael Damkier
Hamburg, Germany
(michael@vondervotteimittiss.de)

*/

static const uint8_t version[] = { 2,0,1,5,0,8,0,7 };


#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include "ESP32_WROOM32_pins.h"
#include "display_constants_176by220.h"


// TFT object 
TFT_eSPI tft = TFT_eSPI();   

/*
function and range switch settings
*/

/*
meter truth table (at 3870 pins)

      RNGa    RNGb    RNGc
.2      H       H       L
2       L       H       H
20      H       L       H
200     L       L       H
2000    H       H       H
20M     L       L       L
2mS     L       H       L
200nS   H       L       L

        Fa      Fb      Fc      Fd
AC      H       x       x       x
V       x       H       L       x
mA      x       L       H       x
kOhm    x       L       L       x   (also depends on RNG) 
dB      x       H       H       x
S       x       L       L       x   (also depends on RNG) 
REL     x       x       x       L

*/

/*
func truth table

                  v
        F F F F   a
        c a d b   l
        
V       0 x x 1   1
mA      1 x x 0   8
kOhm    0 x x 0   0   (also depends on RNG) 
db      1 x x 1   9
S       0 x x 0   0   (also depends on RNG)
AC      x 1 x x
REL     x x 1 x

*/

#define BIT_Fb    0
#define BIT_Fd    1
#define BIT_Fa    2
#define BIT_Fc    3

#define FUNC_V    1
#define FUNC_A    8
#define FUNC_Ohm  0
#define FUNC_dB   9

/*
rng truth table

          R R R
          N N N   v
        B G G G   a
        T a b c   l

.2      x 1 1 0   6
2       x 0 1 1   3
20      x 1 0 1   5
200     x 0 0 1   1
2000    x 1 1 1   7
20M     x 0 0 0   0   (also set Z in dB function)
2mS     x 0 1 0   2
2nS     x 1 0 0   4
BT      1 x x x

*/

#define BIT_RNGc  0
#define BIT_RNGb  1
#define BIT_RNGa  2
#define BIT_BT    3

#define RNG__2    6
#define RNG_2     3
#define RNG_20    5
#define RNG_200   1
#define RNG_2000  7
#define RNG_20M   0
#define RNG_Z     0
#define RNG_mS    2
#define RNG_nS    4


/*
FUNCTIONS
*/

void readFRNG() {

  static boolean redraw = false;

  static boolean previous_rel = false;
  static boolean previous_relValid = false;

  static uint8_t previous_mode = MODE_NONE;
  static uint8_t previous_unit = UNIT_NONE;
  static uint16_t previous_base = BASEUNIT_undef;

  // read function and range switches (and BT)
  // in both ports, only the upper four bits are of interest,
  // shift them into the lower four
  uint8_t func = (digitalRead(BIT_8050_Fc) << 3) | (digitalRead(BIT_8050_Fa)   << 2) | (digitalRead(BIT_8050_Fd)   << 1) | digitalRead(BIT_8050_Fb); 
  uint8_t rng  = (digitalRead(BIT_BT)      << 3) | (digitalRead(BIT_8050_RNGc) << 2) | (digitalRead(BIT_8050_RNGb) << 1) | digitalRead(BIT_8050_RNGa); 

  // set AC flag    
  boolean fAC = test_bit(func,BIT_Fa);
  // set REL flag
  boolean fREL = ! test_bit(func,BIT_Fd); // REL is active low
  // set BT flag
  boolean fBT = test_bit(rng,BIT_BT);
  
  // clear flag bits in func and rng
  clear_bit(func,BIT_Fa); // Fa/AC
  clear_bit(func,BIT_Fd); // Fd/REL
  clear_bit(rng,BIT_BT); // BT
  
  boolean frngValid = true;

  // set up units based on func and rng switches
  switch ( func ) {
    case FUNC_V:
      baseUnit = BASEUNIT_V;
      if ( rng == RNG_20M )
        frngValid = false;
      else if ( rng == RNG__2 )
        unit = UNIT_mV;
      else
        unit = UNIT_V;
      break;
    
    case FUNC_A:
      baseUnit = BASEUNIT_A;
      if ( rng == RNG_20M )
        frngValid = false;
      else if ( rng == RNG__2 )
        unit = UNIT_microA;
      else
        unit = UNIT_mA;
      break;

    case FUNC_Ohm:
      if ( rng == RNG_mS || rng == RNG_nS )
        baseUnit = BASEUNIT_S;
      else
        baseUnit = BASEUNIT_R;
      if ( rng == RNG_mS )
        unit = UNIT_mS;
      else if ( rng == RNG_nS )
        unit = UNIT_nS;
      else if ( rng == RNG_20M )
        unit = UNIT_MOhm;
      else if ( rng == RNG__2 )
        unit = UNIT_Ohm;
      else
        unit = UNIT_kOhm;
      break;

    case FUNC_dB:
      if ( rng == RNG_Z ) {
        baseUnit = BASEUNIT_Z;
        unit = UNIT_Z;
        dp = DP_NONE;
      }
      else {
        baseUnit = BASEUNIT_dB;
        unit = UNIT_dB;
        dp = 2;
      }
      break;
  }
  
  // decimal point position  
  if ( func != FUNC_dB ) {
    switch ( rng ) {
      case RNG_2:
        dp = 0;
        break;
      case RNG_20:
      case RNG_20M:
        dp = 1;
        break;
      case RNG__2:
      case RNG_200:
      case RNG_mS:
      case RNG_nS:
        dp = 2;
        break;
      case RNG_2000:
        dp = 3;
        break;
    }
  }

  // set mode (AC, DC, none)
  if ( func != FUNC_Ohm && baseUnit != BASEUNIT_Z )
    mode = fAC ? MODE_AC : MODE_DC;
  else
    mode = MODE_NONE;
    
  // set rel (REL is on/off)
  rel = fREL;

  // update the display
  if ( frngValid && ! redraw ) {
    // handle REL display
    if ( ! rel ) {
      // if not displaying REL, make sure REL info reflects the current setting
      rUnit = unit;
      rMode = mode;
      dpr = dp;
      rBaseUnit = baseUnit;
    }
    else {
      // REL is on but something may have changed making the rel display invalid
      relValid = rBaseUnit == baseUnit && rMode == mode;
    }
    // refresh mode, unit, Z, rel as appropriate
    if ( baseUnit != previous_base ) {
      previous_base = baseUnit;
      displayMode();
      displayZ();
    }
    if ( mode != previous_mode ) {
      previous_mode = mode;
      displayMode();
    }
    if ( unit != previous_unit ) {
      previous_unit = unit;
      displayUnit();
    }
    if ( rel != previous_rel || relValid != previous_relValid ) {
      previous_rel = rel;
      previous_relValid = relValid;
      displayRel();
    }
    // update main numerical display
    formatDigits();
  }
  else if ( frngValid && redraw ) {
    redraw = false;
    displayMode();
    displayUnit();
    displayRel();
  }
  else if ( ! frngValid && ! redraw ) {
    clearDisplay();
    redraw = true;
  }

  // always display battery status
  displayLoBatt( fBT );
}

void formatDigits() {
  // set BCD for strobe/digit 1-4
  for ( uint8_t i = ST1; i <= ST4; i++ )
  {
    digits[ i ] = scanCodes[ i ] >> 4;
  }
  // set strobe/digit 0
  if ( test_bit(scanCodes[ST0],BIT_1) )
  {
    digits[ 0 ] = 1;
  }
  else
  {
    digits[ 0 ] = DIGIT_SPACE;
  }
  // set sign
  // The 8050A LCD displays both plus and minus signs.
  // On the LCD display, the plus sign is made up of the minus sign
  // and the vertical plus segments.
  // So, to correctly display plus, do that check first.  
  if ( test_bit(scanCodes[ST0],BIT_PLUS) )
  {
    sign = SIGN_PLUS;
  }
  else if ( test_bit(scanCodes[ST0],BIT_MINUS) )
  {
    sign = SIGN_MINUS;
  }
  else
  {
    sign = SIGN_NONE;
  }
  // set HV indication
  hv = test_bit(scanCodes[0],BIT_HV);
  // special handling for setting Z
  if ( baseUnit == BASEUNIT_Z )
  {
    // Z values as displayed on the 8050A
    // 01234
    // -----
    //   50
    //   75
    //   93
    //  110
    //  125
    //  135
    //  150
    //  250
    //  300
    //  500
    //  600
    //  800
    //  900
    // 1000
    // 1200
    //  8   (8000)
    // The 8050A displays the Z set values shifted
    // one place to the left except for 8000 which
    // is displayed as ' 8   '. The test determines
    // if the display is '*8***' (i.e., 8000) but
    // not '*80**' (800).
    if ( digits[ 1 ] == 8 && digits[ 2 ] != 0 )
    {
      zDigits[ 1 ] = 8;
      for ( uint8_t i = 2; i < NUM_DIGITS; i++ )
        zDigits[ i ] = 0;
    }
    else
    {
      memcpy( zDigits + 1, digits, NUM_DIGITS - 1 );
    }
  }
  else if ( ! rel )
  {
    // if not setting Z and not displaying REL,
    // save this as a potential REL value
    rSign = sign;
    memcpy( relDigits, digits, NUM_DIGITS );
  }

  displayMain();

  return;
}

void displayMain() {
  uint16_t x = X_MAIN;
  // sign
//   uint8_t s = baseUnit == BASEUNIT_Z ? UNIT_NONE : sign;
  tft.bmpDraw(SIGN_LG(sign), x, Y_MAIN + OFFSET_SIGN_LG, W_SIGN_LG, H_SIGN_LG, FG_COLOR, BG_COLOR );
  x += W_SIGN_LG;
  // digits
  uint8_t * d = (uint8_t*) (baseUnit == BASEUNIT_Z ? &zDigits : &digits);
  for ( uint8_t i = 0; i < NUM_DIGITS; i++ )
  {
    tft.bmpDraw( 
      DIGIT_LG(d[i]), 
      x, 
      Y_MAIN, 
      W_DIGIT_LG, 
      H_DIGIT_LG, 
      FG_COLOR, 
      BG_COLOR );
    x += W_DIGIT_LG;
    // decimal point
    if ( dp == i ) {
      // DP_NONE is defined as position 4 (far right).
      // Though a decimal point is never displayed there,
      // the position must be drawn to repair any previous
      // digit stuff that was displayed.
      // TODO: Is setting Z the only time when no dp is displayed?
      //       If so, maybe this can be done differently and
      //       DP_NONE can be redefined as 5, e.g. 
      tft.bmpDraw( 
        dp == DP_NONE ? 0 : dp_lg,
        x,
        Y_MAIN,
        W_DP_LG,
        H_DP_LG,
        FG_COLOR,
        BG_COLOR );
      x += W_DP_LG;
    }
  }
  // HV
  tft.bmpDraw(
    0,
    X_UNDERLINE,
    Y_UNDERLINE,
    W_UNDERLINE,
    H_UNDERLINE,
    0,
    hv ? HV_COLOR : FG_COLOR );
}

void displayRel() {
  boolean disp = rel && baseUnit != BASEUNIT_Z;
  boolean dispDigits = disp && relValid;
  uint16_t x = X_REL;
  // REL symbol
  tft.bmpDraw(
    disp ? MODE_LG(MODE_REL) : MODE_NONE,
    x,
    Y_REL, 
    W_MODE, 
    H_MODE,
    rBaseUnit, 
    BG_COLOR );
  x += W_MODE + W_SPACE;
  // sign
  tft.bmpDraw(
    dispDigits ? SIGN_SM(rSign) : SIGN_NONE,
    x,
    Y_REL + OFFSET_SIGN_SM,
    W_SIGN_SM,
    H_SIGN_SM,
    rBaseUnit,
    BG_COLOR );
  x += W_SIGN_SM;
  // digits
  for ( uint8_t i = 0; i < 5; i++ )
  {
    tft.bmpDraw( 
      dispDigits ? DIGIT_SM(relDigits[i]) : 0, 
      x, 
      Y_REL, 
      W_DIGIT_SM, 
      H_DIGIT_SM,
      rBaseUnit, 
      BG_COLOR );
    x += W_DIGIT_SM;
    if ( dpr == i ) {
      tft.bmpDraw( 
        dispDigits && dpr != DP_NONE ? dp_sm : 0,
        x,
        Y_REL,
        W_DP_SM,
        H_DP_SM,
        rBaseUnit,
        BG_COLOR );
      x += W_DP_SM;
    }
  }
  x += W_SPACE;
  // unit  
  tft.bmpDraw(
    disp ? UNIT_SM(rUnit) : UNIT_NONE,
    x,
    Y_REL + OFFSET_UNIT_SM,
    W_UNIT_SM,
    H_UNIT_SM,
    rBaseUnit,
    BG_COLOR );
  // mode
  tft.bmpDraw(
    disp ? MODE_SM(rMode) : MODE_NONE,
    x,
    Y_REL + OFFSET_MODE_SM,
    W_MODE_SM,
    H_MODE_SM,
    rBaseUnit,
    BG_COLOR );
}

void displayZ() {
  if ( baseUnit == BASEUNIT_Z ) {
    tft.bmpDraw(
      symbolSetZ,
      X_Z,
      Y_Z,
      W_SET_Z, // =64=4*W_DIGIT_SM
      H_SET_Z,
      BASEUNIT_Z,
      BG_COLOR );
    tft.bmpDraw(
      0,
      X_Z+W_SET_Z,
      Y_Z,
      W_DIGIT_SM+W_SPACE+W_UNIT_SM, // =16+12+24=52
      OFFSET_UNIT_SM+H_UNIT_SM,
      BASEUNIT_Z,
      BG_COLOR );
  }
  else if ( baseUnit == BASEUNIT_dB ) {
    uint16_t x = X_Z;
    for ( uint8_t i = 0; i < NUM_DIGITS; i++ )
    {
      tft.bmpDraw(
        DIGIT_SM(zDigits[i]),
        x,
        Y_Z,
        W_DIGIT_SM,
        H_DIGIT_SM,
        BASEUNIT_Z,
        BG_COLOR );
      x += W_DIGIT_SM;
    }
    x += W_SPACE;
    // unit  
    tft.bmpDraw(
      UNIT_SM(UNIT_Z),
      x,
      Y_Z + OFFSET_UNIT_SM,
      W_UNIT_SM,
      H_UNIT_SM,
      BASEUNIT_Z,
      BG_COLOR );
  }
  else {
    tft.bmpDraw(
      0,
      X_Z,
      Y_Z,
      W_SET_Z + W_DIGIT_SM+W_SPACE+W_UNIT_SM, // =64=4*W_DIGIT_SM
      OFFSET_UNIT_SM+H_UNIT_SM,
      BASEUNIT_Z,
      BG_COLOR );
  }
  
}

void displayMode() {
  tft.bmpDraw(
    MODE_LG(mode),
    X_MODE,
    Y_MODE,
    W_MODE,
    H_MODE,
    baseUnit,
    BG_COLOR );
}

void displayUnit() {
  tft.bmpDraw( 
    UNIT_LG(unit),
    X_UNIT,
    Y_UNIT,
    W_UNIT,
    H_UNIT,
    baseUnit,
    BG_COLOR );
}

void displayLoBatt( boolean loBatt ) {
  tft.bmpDraw(
    loBatt ? symbolLoBatt : MODE_NONE,
    X_LOBATT,
    Y_LOBATT,
    W_LOBATT,
    H_LOBATT,
    BT_COLOR,
    BG_COLOR );
}

void clearDisplay() {
  tft.fillScreen( BG_COLOR );
  for ( uint16_t x = X_MAIN + W_SIGN_LG + W_DIGIT_LG;
        x < X_MAIN + W_SIGN_LG + (5 * W_DIGIT_LG);
        x += W_DIGIT_LG ) {
    tft.bmpDraw(
      dp_lg,
      x,
      Y_MAIN,
      W_DP_LG,
      H_DP_LG,
      FG_COLOR,
      BG_COLOR );
  }
}

void splash() {
  uint16_t x = 40;
  uint16_t y = 30;
  uint8_t n = sizeof( version );
  tft.fillScreen( BG_COLOR );
  tft.bmpDraw(
    splashFluke,
    x,
    y,
    W_SPLASH_FLUKE,
    H_SPLASH_FLUKE,
    FG_COLOR,
    BG_COLOR );
  y += H_SPLASH_FLUKE + 10;
  tft.bmpDraw(
    splashName,
    x,
    y,
    W_SPLASH_NAME,
    H_SPLASH_NAME,
    FG_COLOR,
    BG_COLOR );
  x = 310 - (n + 1) * W_DIGIT_TINY - W_SPLASH_PROGNAME;
  y = 230 - H_DIGIT_TINY;
  tft.bmpDraw(
    splashProgName,
    x,
    y,
    W_SPLASH_PROGNAME,
    H_SPLASH_PROGNAME,
    FG_COLOR,
    BG_COLOR );
  x = 310 - n * W_DIGIT_TINY;
  for ( uint8_t i = 0; i < n; i++ ) {
    tft.bmpDraw(
      DIGIT_TINY( version[ i ] ),
      x,
      y,
      W_DIGIT_TINY,
      H_DIGIT_TINY,
      FG_COLOR,
      BG_COLOR );
    x += W_DIGIT_TINY;
  }
  x = 10;
  y = 230 - H_SPLASH_ARDUINO;
  tft.bmpDraw(
    splashArduino,
    x,
    y,
    W_SPLASH_ARDUINO,
    H_SPLASH_ARDUINO,
    0x0C0F,
    BG_COLOR );
  y -= H_SPLASH_ADAFRUIT + 5;
  tft.bmpDraw(
    splashAdafruit,
    x,
    y,
    W_SPLASH_ADAFRUIT,
    H_SPLASH_ADAFRUIT,
    FG_COLOR,
    BG_COLOR );
  delay( 2000 );
  tft.fillScreen( BG_COLOR );
}
// interrupt service routines
// - reads the scancode values from the 8050A
// - sets the strobeCycle flag when all strobes are read
// 
// When making measurements, the meter strobes at ~410mS.
// When setting Z, there is a set of two strobes spaced
// at ~2mS with 1S between sets. (The desired value is
// for the second strobe in the set.)
// When the frng is invalid (e.g., 20MOhm is selected when
// in V), the strobes are at ~12mS.
// 
// ST0
ISR(INT3_vect) {
  scanCodes[ ST0 ] = PINF;
}
// ST1
ISR(INT2_vect) {
  scanCodes[ ST1 ] = PINF;
}
// ST2
ISR(INT1_vect) {
  scanCodes[ ST2 ] = PINF;
}
// ST3
ISR(INT0_vect) {
  scanCodes[ ST3 ] = PINF;
}
// ST4
ISR(INT6_vect) {
  scanCodes[ ST4 ] = PINF;
  strobeCycle = true;
}




// the setup and loop

void setup(void) {
  Serial.begin(115200);  
  // 
  pinMode(BIT_8050_RNGc, INPUT);
  pinMode(BIT_8050_RNGb, INPUT);
  pinMode(BIT_8050_RNGa, INPUT);
  pinMode(BIT_8050_BT, INPUT);

  // func 
  pinMode(BIT_8050_Fb, INPUT);
  pinMode(BIT_8050_Fd, INPUT);
  pinMode(BIT_8050_Fa, INPUT);
  pinMode(BIT_8050_Fc, INPUT);

  // 8050A strobe lines
  pinMode(BIT_ST0, INPUT);
  pinMode(BIT_ST1, INPUT);
  pinMode(BIT_ST2, INPUT);
  pinMode(BIT_ST3, INPUT);
  pinMode(BIT_ST4, INPUT);

  // 8050A scancode lines
  pinMode(BIT_HV, INPUT);
  pinMode(BIT_DP, INPUT);
  pinMode(BIT_Z, INPUT);
  pinMode(BIT_Y, INPUT);
  pinMode(BIT_X, INPUT);
  pinMode(BIT_W, INPUT);

  pinMode(TFT_SCLK, OUTPUT);
  pinMode(TFT_MOSI, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_RST, OUTPUT);
  pinMode(TFT_CS, OUTPUT);

  // initialize display
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
}

void loop(void) {
  if ( strobeCycle ) {
    strobeCycle = false;
    readFRNG();
  }
}

