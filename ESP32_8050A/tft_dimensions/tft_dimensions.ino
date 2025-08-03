#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define TFT_ROTATION 1

void setup() {
  Serial.begin(115200);
  Serial.println("TFT Dimensions Test");
  
  // Initialize the TFT display
  tft.init();
  tft.setRotation(TFT_ROTATION); // Set rotation to match your config
  
  // Get display dimensions
  int displayWidth = tft.width();
  int displayHeight = tft.height();
  
  Serial.println("=== TFT Display Dimensions ===");
  Serial.print("Display Width: ");
  Serial.println(displayWidth);
  Serial.print("Display Height: ");
  Serial.println(displayHeight);
  Serial.print("Total Pixels: ");
  Serial.println(displayWidth * displayHeight);
  
  // Test sprite dimensions too
  TFT_eSprite testSprite = TFT_eSprite(&tft);
  testSprite.createSprite(50, 30);
  
  Serial.println("\n=== Sprite Dimensions ===");
  Serial.print("Sprite Width: ");
  Serial.println(testSprite.width());
  Serial.print("Sprite Height: ");
  Serial.println(testSprite.height());
  
  // Display the dimensions on screen
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  
  tft.setCursor(10, 10);
  tft.print("Width: ");
  tft.println(displayWidth);
  
  tft.setCursor(10, 50);
  tft.print("Height: ");
  tft.println(displayHeight);
  
  tft.setCursor(10, 90);
  tft.print("Rotation: ");
  tft.println( TFT_ROTATION );

  Serial.println("\nDimensions displayed on screen!");
}

void loop() {
  // Nothing to do in loop
  delay(1000);
} 