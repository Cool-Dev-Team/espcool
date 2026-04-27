#include <M5Unified.h>

const uint16_t rainbowColors[7] = { 
  TFT_RED, 
  TFT_ORANGE, 
  TFT_YELLOW, 
  TFT_GREEN, 
  TFT_CYAN, 
  TFT_BLUE, 
  TFT_PURPLE 
};

// Declare playMelody() before it's used
void playMelody();

void drawRainbow() {
  for (int i = 0; i < 7; i++) {
    M5.Lcd.fillRect(i * 19, 20, 19, 5, rainbowColors[i]);
  }
}

void playMelody() {
  const int notes[] = {262, 294, 330, 349, 392, 440, 494};
  for (int i = 0; i < 7; i++) {
    M5.Speaker.tone(notes[i], 200);
    delay(250);
  }
  M5.Speaker.tone(523, 500);
  delay(600);
  M5.Speaker.stop();
}

// Main setup function declarations remain as is:
// 1. Proper function order
// 2. Correct display initialization

void setup() {
  M5.begin();
  M5.Lcd.setRotation(0);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("Flasher Test Success!\n");
  drawRainbow();  // Now recognizes drawRainbow()
  playMelody();
}

void loop() {
  // ... existing loop code remains ...
}
