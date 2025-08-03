// Simple function to get TFT dimensions
void getTFTDimensions() {
  // Get display dimensions
  int displayWidth = tft.width();
  int displayHeight = tft.height();
  
  // Print to Serial Monitor
  Serial.println("=== TFT Dimensions ===");
  Serial.print("Width: ");
  Serial.println(displayWidth);
  Serial.print("Height: ");
  Serial.println(displayHeight);
  Serial.print("Total Pixels: ");
  Serial.println(displayWidth * displayHeight);
  
  // You can also return these values or store them in global variables
  // return displayWidth, displayHeight; // Note: C++ doesn't support multiple return values
}

// Alternative: Functions that return individual dimensions
int getTFTWidth() {
  return tft.width();
}

int getTFTHeight() {
  return tft.height();
}

// Function to get sprite dimensions
void getSpriteDimensions(TFT_eSprite &sprite) {
  Serial.println("=== Sprite Dimensions ===");
  Serial.print("Sprite Width: ");
  Serial.println(sprite.width());
  Serial.print("Sprite Height: ");
  Serial.println(sprite.height());
}

// Usage example in setup():
/*
void setup() {
  Serial.begin(115200);
  
  tft.init();
  tft.setRotation(1);
  
  // Get and display dimensions
  getTFTDimensions();
  
  // Or get individual dimensions
  int w = getTFTWidth();
  int h = getTFTHeight();
  Serial.print("Individual: ");
  Serial.print(w);
  Serial.print(" x ");
  Serial.println(h);
}
*/ 