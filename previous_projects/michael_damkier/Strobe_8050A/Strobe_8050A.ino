/*
Strobe_8050A.ino

Display strobe simulator for Fluke 8050A multimeter

2013.07.15
prototype 1

Michael Damkier
Hamburg, Germany
(michael@vondervotteimittiss.com)

*/

// remove this #define to turn off serial print calls
#define DEBUG

#ifdef DEBUG
#define DEBUG_INIT Serial.begin(9600)
#define DEBUG_PRINT(str) Serial.print(str)
#define DEBUG_PRINTLN(str) Serial.println(str)
#else
#define DEBUG_INIT
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

// register bit clear/set code suggested on the Arduino forum
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif



#define PULSE_WIDTH 100 // microSeconds
#define INTER_PULSE_TIME 200 // microSeconds
#define REFRESH_PERIOD 200 // milliSeconds


/*
scan code output
*/

// define the pins
#define PIN_ST0 8
#define PIN_ST1 9
#define PIN_ST2 10
#define PIN_ST3 11
#define PIN_ST4 12

#define PIN_Z 2
#define PIN_Y 3
#define PIN_X 4
#define PIN_W 5

#define PIN_DP 6
#define PIN_HV 7

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
#define SCANCODE_DP 4
#define SCANCODE_HV 5

#define SCANCODE_DB SCANCODE_Y
#define SCANCODE_1 SCANCODE_Z
#define SCANCODE_MINUS SCANCODE_W
#define SCANCODE_PLUS SCANCODE_X
#define SCANCODE_REL SCANCODE_DP

static const uint8_t StrobePins[ NUM_STROBES ] = {
  PIN_ST0,
  PIN_ST1,
  PIN_ST2,
  PIN_ST3,
  PIN_ST4
};

static const uint8_t ScanCodePins[ NUM_SCANCODES ] = {
  PIN_Z,
  PIN_Y,
  PIN_X,  
  PIN_W,
  PIN_DP,
  PIN_HV
};

boolean loBatt = false;


/*
GLOBALS
*/

uint8_t st; // strobe position

uint8_t scanCodes[ NUM_STROBES ][ NUM_SCANCODES ];



#if defined (DEBUG)
void printScanCodes()
{
  DEBUG_PRINTLN();
  DEBUG_PRINT( "scan codes" );
  for ( uint8_t i = ST0; i < NUM_STROBES; i++ )
  {
    DEBUG_PRINTLN();
    for ( uint8_t j = SC0; j < NUM_SCANCODES; j++ )
    {
      DEBUG_PRINT( scanCodes[ i ][ j ] );
      DEBUG_PRINT( "." );
    }
  }
  DEBUG_PRINTLN();
  return;
}
#endif // defined DEBUG


void pulse( uint8_t pin )
{
  digitalWrite( pin, HIGH );
  delayMicroseconds( PULSE_WIDTH );
  digitalWrite( pin, LOW );
  delayMicroseconds( INTER_PULSE_TIME );
}

String text = "";
boolean newText = false;

void clearScanCodes()
{
  for ( uint8_t i = ST0; i < NUM_STROBES; i++ )
  {
    for ( uint8_t j = SC0; j < NUM_SCANCODES; j++ )
      scanCodes[ i ][ j ] = 0;
  }
  for ( uint8_t i = ST1; i < NUM_STROBES; i++ )
  {
    for ( uint8_t j = SC0; j <= SC3; j++ )
      scanCodes[ i ][ j ] = 1;
  }
}

void bcd( uint8_t s, char c )
{
  c -= 48;
  uint8_t mask = B00000001;
  for ( uint8_t i = SCANCODE_Z; i <= SCANCODE_W; i++ )
  {
    scanCodes[ s ][ i ] = c & mask ? 1 : 0;
    mask <<= 1;
  }
}

void setScanCodes()
{
  if ( text.equals( "db" ) )
  {
    scanCodes[ ST0 ][ SCANCODE_DB ] = scanCodes[ ST0 ][ SCANCODE_DB ] ? 0 : 1;
  }
  if ( text.equals( "hv" ) )
  {
    scanCodes[ ST0 ][ SCANCODE_HV ] = scanCodes[ ST0 ][ SCANCODE_HV ] ? 0 : 1;
  }
  if ( text.equals( "rel" ) )
  {
    scanCodes[ ST0 ][ SCANCODE_REL ] = scanCodes[ ST0 ][ SCANCODE_REL ] ? 0 : 1;
  }
  if ( text.equals( "clr" ) )
  {
    clearScanCodes();
  }
  if ( text.equals( "err" ) )
  {
    clearScanCodes();
    for ( uint8_t i = ST1; i <= ST4; i++ )
      scanCodes[ i ][ SCANCODE_DP ] = 1; 
  }
  if ( text.length() >= 6 )
  {
    uint8_t i = 0;
    clearScanCodes();
    if ( text[ i ] == '-' )
    {
      scanCodes[ ST0 ][ SCANCODE_MINUS ] = 1;
      i++;
    }
    if ( text[ i ] == '1' )
    {
      scanCodes[ ST0 ][ SCANCODE_1 ] = 1;
    }
    for ( uint8_t j = ST1; j <= ST4; j++ )
    {
      i++;
      if ( text[ i ] == '.' )
      {
        scanCodes[ j ][ SCANCODE_DP ] = 1;
        i++;
      }
      bcd( j, text[ i ] );
    }
  }
  
  return;
}


// The Arduino setup and loop

void setup()
{
  DEBUG_INIT;

  DEBUG_PRINTLN();
  DEBUG_PRINTLN("Strobe 8050A");
  DEBUG_PRINT("Setting up...");

  for ( uint8_t i = 0; i < NUM_STROBES; i++ )
  {
    pinMode( StrobePins[ i ], OUTPUT );
    digitalWrite( StrobePins[ i ], LOW );
  }

  for ( uint8_t i = 0; i < NUM_SCANCODES; i++ )
  {
    pinMode( ScanCodePins[ i ], OUTPUT );
    digitalWrite( ScanCodePins[ i ], LOW );
  }
  
  clearScanCodes();

  DEBUG_PRINTLN("done");
}

void loop()
{
  for ( uint8_t i = 0; i < NUM_STROBES; i++ )
  {
    for ( uint8_t j = 0; j < NUM_SCANCODES; j++ )
    {
      digitalWrite( ScanCodePins[ j ], scanCodes[ i ][ j ] );
    }
    pulse( StrobePins[ i ] );
  }
  delay( REFRESH_PERIOD );

  if ( newText )
  {
    Serial.println();
    Serial.println( text );
    setScanCodes();
    text = "";
    newText = false;
  }

#if defined (DEBUG)
  printScanCodes();
#endif // defined DEBUG

}

void serialEvent()
{
  while ( Serial.available() )
  {
    char in = (char) Serial.read();
    if ( in == '\n' )
      newText = true;
    else
      text += in;
  }
}



