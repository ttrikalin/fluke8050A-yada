/*
LED_8050A.ino

LED display implementation for Fluke 8050A multimeter


2014.04.18
fixed a bug where the set value of Z is wrong
---------------------------------------------
When setting REF Z, the meter cycles through
the available values. The LED display was one
value behind. For example, if the value chosen
was 1000, the value in the meter was the next
one, i.e., 1100.
This was caused by not taking into account the
strobe timing when setting REF Z where the strobes
come in two sets spaced only 2mS apart. The second
set, which represents the new value, was lost because
the updateDisplay() routine did the display update
before resetting the displayUpdate flag.
This was fixed in the updateDisplay() routine by
moving the displayUpdate flag reset to just after
the test, before the actual update code is called.

 
2013.08.12
prototype 3

Michael Damkier
Hamburg, Germany
(michael@vondervotteimittiss.de)

*/


/*
PIN ASSIGNMENTS
*/

// MAX7219 communication pins
#define PIN_LOAD 8
#define PIN_CLK 9
#define PIN_DIN 10

// 8050A strobe lines
#define PIN_ST0 A0
#define PIN_ST1 A1
#define PIN_ST2 A2
#define PIN_ST3 A3
#define PIN_ST4 A4

// 8050A scancode lines
#define PIN_Z 2
#define PIN_Y 3
#define PIN_X 4
#define PIN_W 5
#define PIN_HV 6
#define PIN_DP 7

/*
CONSTANTS
*/

// strobe lines
#define NUM_STROBES 5
#define ST0 0
#define ST1 1
#define ST2 2
#define ST3 3
#define ST4 4

// scancode lines
#define NUM_SCANCODES 6
#define SC0 0
#define SC1 1
#define SC2 2
#define SC3 3
#define SC4 4
#define SC5 5

#define SCANCODE_Z 0
#define SCANCODE_Y 1
#define SCANCODE_X 2
#define SCANCODE_W 3
#define SCANCODE_HV 4
#define SCANCODE_DP 5

#define SCANCODE_DB SCANCODE_Y
#define SCANCODE_1 SCANCODE_Z
#define SCANCODE_MINUS SCANCODE_W
#define SCANCODE_PLUS SCANCODE_X
#define SCANCODE_REL SCANCODE_DP

// pin arrays
static const uint8_t StrobePins[ NUM_STROBES ] = {
  PIN_ST0,
  PIN_ST1,
  PIN_ST2,
  PIN_ST3,
  PIN_ST4
};

static const uint8_t ScanCodePins[ NUM_SCANCODES ] = {
  PIN_Z, // 2^0
  PIN_Y, // 2^1
  PIN_X, // 2^2
  PIN_W, // 2^3
  PIN_HV,
  PIN_DP
};

// Pin Change Interrupts
// Enable pin change interrupts in the PCICR Pin Change Interrupt Control Register
// by setting the appropriate PCIEn Pin Change Interrupt Enable bit(s)
//   PCIE0 enables Arduino pins 8-13, mask bits PCINT0-5
//   PCIE1 enables Arduino pins A0-A5, mask bits PCINT8-13
//   PCIE2 enables Arduino pins 0-7, mask bits PCINT16-23
// Set the desired pin(s) in the appropriate PCMSKn Pin Change Mask Register
//   PCMSK0 handles bits PCINT0-5
//   PCMSK1 handles bits PCINT8-13
//   PCMSK2 handles bits PCINT16-23
// for example, enable a pin change interrupt on Arduino pin 7
// PCICR = _BV( PCIE2 );
// PCMSK2 = _BV( PCINT23 );
//
//------------------------------------------------------------------
// Arduino pins for the 8050A strobe lines are defined in PIN_ST0-4.
// Adjust the following values, accordingly.
//------------------------------------------------------------------
//
// the control register interrupt enable bit
#define STROBE_INTERRUPT_ENABLE _BV( PCIE1 )
// pin change mask register
#define STROBE_INTERRUPT_MASK_REGISTER PCMSK1
// The ISR is called on the vector PCINTn_vect, corresponding PCIEn.
#define STROBE_INTERRUPT_VECTOR PCINT1_vect
// This array maps the Arduino pins used for the strobes to
// the appropriate bit-mask for setting the PCMSKn register.
static const uint8_t StrobePin2PCINT[ NUM_STROBES ] = {
  _BV( PCINT8 ),
  _BV( PCINT9 ),
  _BV( PCINT10 ),
  _BV( PCINT11 ),
  _BV( PCINT12 )
};

// MAX7219 registers
#define REGISTER_NOOP 0x00
#define REGISTER_DIGIT_0 0x01
#define REGISTER_DIGIT_1 0x02
#define REGISTER_DIGIT_2 0x03
#define REGISTER_DIGIT_3 0x04
#define REGISTER_DIGIT_4 0x05
#define REGISTER_DIGIT_5 0x06
#define REGISTER_DIGIT_6 0x07
#define REGISTER_DIGIT_7 0x08
#define REGISTER_DECODE_MODE 0x09
#define REGISTER_INTENSITY 0x0A
#define REGISTER_SCAN_LIMIT 0x0B
#define REGISTER_SHUTDOWN 0x0C
#define REGISTER_DISPLAY_TEST 0x0F

// MAX7219 register values
#define REGISTER_DATA_DECODE_MODE B11011110 // all Code B except digits 5,0
#define REGISTER_DATA_SCAN_LIMIT 0x05 // scan digits 0-5
#define REGISTER_DATA_INTENSITY 0x0F // 31/32, full intensity

// number of digits in the display
#define NUM_DIGITS 6

// MAX7219 segment mapping
#define LED_SEGMENT_A B01000000
#define LED_SEGMENT_B B00100000
#define LED_SEGMENT_C B00010000
#define LED_SEGMENT_D B00001000
#define LED_SEGMENT_E B00000100
#define LED_SEGMENT_F B00000010
#define LED_SEGMENT_G B00000001
#define LED_SEGMENT_DP B10000000
#define LED_SEGMENT_ALL B01111111
#define LED_SEGMENT_NONE B00000000

// MAX7219 Code B mapping
#define LED_CODE_BLANK B00001111

/*
GLOBALS
*/

// strobe position (ISR)
volatile uint8_t st;

// 8050A scancode buffer (ISR)
volatile uint8_t scanCodes[ NUM_STROBES ][ NUM_SCANCODES ];

// update display flag (ISR)
volatile boolean displayUpdate = false;

// MAX7219 digit data
uint8_t digits[ NUM_DIGITS ] = {
  LED_SEGMENT_NONE,
  LED_CODE_BLANK,
  LED_CODE_BLANK,
  LED_CODE_BLANK,
  LED_CODE_BLANK,
  LED_SEGMENT_NONE
}; // (digits 0,5 use no decode, 1-4 use Code B)

// low-battery indicator
// boolean loBatt = false;

/*
FUNCTIONS
*/

// pin change interrupt service routine
// - reads the scancode values from the 8050A
// - sets the displayUpdate flag when all strobes are read
ISR( STROBE_INTERRUPT_VECTOR )
{
  if ( digitalRead( StrobePins[ st ] ) == LOW )
  {
    // that was a falling transition
    // (In the 4054/55/56 LCD drivers datasheet, it shows
    // the data being latched on the strobe falling edge.)
    // now, read the scan code values
    for ( uint8_t i = SC0; i < NUM_SCANCODES; i++ )
    {
      scanCodes[ st ][ i ] = digitalRead( ScanCodePins[ i ] );
    }
    // next time, next strobe
    st++;
    if ( st >= NUM_STROBES )
    {
      // scancodes for all strobes are read
      st = ST0; // reset strobe counter
      // now is the time to update the display
      displayUpdate = true;
    }
    // enable the appropriate strobe pin
    STROBE_INTERRUPT_MASK_REGISTER = StrobePin2PCINT[ st ];
  }
}

void formatDigits()
{
  // clear digits
  for ( uint8_t i = 0; i < NUM_DIGITS; i++ )
  {
    digits[ i ] = 0;
  }
  // set decimal points 
  // In the 8050A LCD display the dp is to the left of the digit, i.e., strobe 1-4.
  // In the LED display, the dp is to the right, so map to digits 0-3.
  for ( uint8_t i = ST1; i <= ST4; i++ )
  {
    if ( scanCodes[ i ][ SCANCODE_DP ] )
    {
      digits[ i - 1 ] |= LED_SEGMENT_DP;
    }
  }
  // set BCD for strobe/digit 1-4
  // In the LED driver these digits use Code B (refer to MAX7219 datasheet).
  uint8_t code;
  uint8_t mask;
  for ( uint8_t i = ST1; i <= ST4; i++ )
  {
    code = SCANCODE_W; // MSB
    mask = B00001000;
    while ( mask )
    {
      if ( scanCodes[ i ][ code ] )
        digits[ i ] |= mask;
      code--;
      mask >>= 1;
    }
  }
  // set digit 0 (minus, 1, indicator BT)
  // In the LED driver this digit uses no segment decode.
  if ( scanCodes[ ST0 ][ SCANCODE_MINUS ] && ! scanCodes[ ST0 ][ SCANCODE_PLUS ] )
  {
  	// The 8050A LCD displays both minus and plus signs.
  	// The plus sign is made up of the minus sign and the vertical plus segments.
  	// So, to correctly display minus, check that it is not on as a part of plus.  
    digits[ 0 ] |= LED_SEGMENT_G;
  }
  if ( scanCodes[ ST0 ][ SCANCODE_1 ] )
  {
    digits[ 0 ] |= (LED_SEGMENT_B | LED_SEGMENT_C);
  }
//   if ( loBatt )
//   {
//     digits[ 0 ] |= LED_SEGMENT_A;
//   }
  // set digit 5 (indicators dB, HV, REL)
  // In the LED driver this digit uses no segment decode.
  if ( scanCodes[ ST0 ][ SCANCODE_DB ] )
  {
    digits[ 5 ] |= LED_SEGMENT_A;
  }
  if ( scanCodes[ ST0 ][ SCANCODE_HV ] )
  {
    digits[ 5 ] |= LED_SEGMENT_G;
  }
  if ( scanCodes[ ST0 ][ SCANCODE_REL ] )
  {
    digits[ 5 ] |= LED_SEGMENT_D;
  }  
  return;
}

// MAX7219
void writeRegister( uint8_t regAddr, uint8_t data ) {
  // combine the address and data to a 16-bit value
  uint16_t reg = regAddr;
  reg <<= 8;
  reg += data;
  uint16_t mask = 0x8000;
  // (from the datasheet)
  // For the MAX7219, serial data at DIN, sent in 16-bit packets,
  // is shifted into the internal 16-bit shift register with each
  // rising edge of CLK regardless of the state of LOAD. The data
  // is then latched...on the rising edge of LOAD.
  digitalWrite( PIN_LOAD, LOW );
  while ( mask )
  {
    digitalWrite( PIN_CLK, LOW );
    digitalWrite( PIN_DIN, reg & mask ? HIGH : LOW );
      // The test to HIGH/LOW is needed because reg is 16-bit
      // and function expects (truncates to) 8-bit. 
    digitalWrite( PIN_CLK, HIGH );
    mask >>= 1;
  }
  digitalWrite( PIN_LOAD, HIGH );
}

// MAX7219
void writeDigits()
{
  for ( uint8_t i = 0, j = REGISTER_DIGIT_0; i < NUM_DIGITS; i++, j++ )
  {
    writeRegister( j, digits[ i ] );
  }
}

void updateDisplay()
{
  // displayUpdate flag is set in the ISR
  if ( displayUpdate )
  {
    displayUpdate = false;
    formatDigits();
    writeDigits();
  }
}  


// The Arduino setup and loop

void setup(void)
{
  // disable ADC (to reduce power)
  ADCSRA = 0; 
  // initialize strobe and scancode pins
  for ( uint8_t i = ST0; i < NUM_STROBES; i++ )
  {
    pinMode( StrobePins[ i ], INPUT );
  }
  for ( uint8_t i = 0; i < NUM_SCANCODES; i++ )
  {
    pinMode( ScanCodePins[ i ], INPUT );
  }
  // initialize MAX7219 pins
  pinMode( PIN_DIN, OUTPUT );
  pinMode( PIN_LOAD, OUTPUT );
  pinMode( PIN_CLK, OUTPUT );

  // initialize MAX7219 registers
  writeRegister( REGISTER_SCAN_LIMIT, REGISTER_DATA_SCAN_LIMIT );
  writeRegister( REGISTER_DECODE_MODE, REGISTER_DATA_DECODE_MODE );
  writeRegister( REGISTER_INTENSITY, REGISTER_DATA_INTENSITY );
  // load initial digit data
  writeDigits();
  // turn on the display
  writeRegister( REGISTER_SHUTDOWN, 0x01 );

  // enable pin change interrupts
  cli();
  st = ST0; // initialize the strobe counter
  PCICR = STROBE_INTERRUPT_ENABLE;
  STROBE_INTERRUPT_MASK_REGISTER = StrobePin2PCINT[ st ];
  sei();

}

void loop(void)
{
  updateDisplay();
}

