#define USER_SETUP_INFO "User_Setup"

#define ILI9225_DRIVER



// #define TFT_MISO -1 // not needed when there is no touch screen 
// #define TFT_SCLK 18
// #define TFT_MOSI 23 // Also called SDI 
// #define TFT_DC    2  // Data Command or register select (RS) control pin
// #define TFT_RST   4  // Reset pin (could connect to RST pin)
// //#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
// #define TFT_CS   15  // Chip select control pin


#define TFT_MISO -1 // not needed when there is no touch screen 
#define TFT_SCLK 21
#define TFT_MOSI  3 // Also called SDI 
#define TFT_DC    1  // Data Command or register select (RS) control pin
#define TFT_RST  22  // Reset pin (could connect to RST pin)
//#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
#define TFT_CS   23  // Chip select control pin


#define LOAD_FONT2 // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters

// Comment out the #define below to stop the SPIFFS filing system and smooth font code being loaded
// this will save ~20kbytes of FLASH
// #define SMOOTH_FONT

#define SPI_FREQUENCY  27000000

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY 20000000

// The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
#define SPI_TOUCH_FREQUENCY 2500000

