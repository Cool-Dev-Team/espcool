#include <Arduino.h>

// ESP32-C3 DevKit M-1 has an onboard LED on GPIO8
#define LED_PIN 8

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("ESP32-C3 LED Blink Started!");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // Turn LED on
  Serial.println("LED ON");
  delay(1000);                   // Wait 1 second
  
  digitalWrite(LED_PIN, LOW);    // Turn LED off
  Serial.println("LED OFF");
  delay(1000);                   // Wait 1 second
}
