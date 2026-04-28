#include <Arduino.h>
#include <WiFi.h>

// ============================================
// ESP32-S3 Firmware — Example Entry Point
// ============================================

#define LED_BUILTIN 2
#define SERIAL_BAUD 115200

// WiFi credentials (replace with your own)
const char* WIFI_SSID     = "YourSSID";
const char* WIFI_PASSWORD = "YourPassword";

void connectWiFi() {
    Serial.println("[WiFi] Connecting...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
        delay(500);
        Serial.print(".");
        retries++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.print("[WiFi] Connected! IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println();
        Serial.println("[WiFi] Connection failed.");
    }
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    
    Serial.println("==========================");
    Serial.println("  ESP32-S3 Firmware v1.0  ");
    Serial.println("==========================");
    
    // Initialize built-in LED
    pinMode(LED_BUILTIN, OUTPUT);
    
    // Connect to WiFi
    connectWiFi();
    
    Serial.println("[Setup] Complete!");
}

void loop() {
    // Blink LED to show firmware is running
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    
    // Print uptime every 10 seconds
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 10000) {
        lastPrint = millis();
        Serial.printf("[Uptime] %lu seconds\n", millis() / 1000);
        Serial.printf("[Heap] Free: %d bytes\n", ESP.getFreeHeap());
    }
}
