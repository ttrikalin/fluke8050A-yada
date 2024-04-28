/*
TFT_8050A.ino for an ESP32_WROOM32 devkit 

TFT display implementation for Fluke 8050A multimeter. 
TA Trikalinos 

Based in part on work by Michael Damkier, Hamburg, Germany (michael@vondervotteimittiss.de), who
provided the decoding of the strobe lines and the function and range truth tables for the Fluke 8050A
*/

#include "TFT_8050A.h"

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


// the setup and loop
void setup(void) {
  initialize_ESP32_WROOM32();
  range_monitor_initialize(); 
  function_monitor_initialize();
  values_monitor_initialize();
  initialize_display(); 
}

void loop(void) {
  delay(10);
  range_monitor_tasks();
  function_monitor_tasks();
  values_monitor_tasks();
  // display_task(); 
}

