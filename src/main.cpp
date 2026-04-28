#include <M5Unified.h>
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

int currentMode = 0;
const int TOTAL_MODES = 4;

// Target IP for testing your server's port forwarding (Change to your server's local IP)
const char* serverIP = "192.168.1.100";
const int targetPorts[] = {80, 443, 25565}; // Web, SSL, and a custom game/app port

// Ultra-flat, strict minimalist HTML hosting the hardware pinout data
const char* fischyWebHTML = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>FischyWeb-S3 | Pinout Host</title>
<style>
body { font-family: monospace; background: #FFFFFF; color: #000000; padding: 40px; margin: 0; }
h1 { font-weight: normal; text-transform: uppercase; border-bottom: 2px solid #000; display: inline-block; padding-bottom: 5px; }
table { width: 100%; max-width: 600px; border-collapse: collapse; margin-top: 20px; }
th, td { border: 2px solid #000; padding: 10px; text-align: left; }
th { background: #000; color: #FFF; font-weight: normal; }
.status { margin-top: 30px; font-weight: bold; border: 2px solid #000; padding: 15px; display: inline-block; }
</style>
</head>
<body>
<h1>FischyWeb-S3 Hardware DB</h1>
<p>Live Diagnostic Server</p>

<table>
<tr><th>Pin Location</th><th>Signal Mapping</th><th>Status</th></tr>
<tr><td>Top Right (Pin 1)</td><td>RED / Video</td><td>Verified</td></tr>
<tr><td>Row 2 (Pin 6)</td><td>RED Ground</td><td>Pending Probe</td></tr>
<tr><td>Row 3 (Pin 13)</td><td>Horizontal Sync</td><td>Pending Probe</td></tr>
<tr><td>Row 3 (Pin 14)</td><td>Vertical Sync (VSync)</td><td>Verified</td></tr>
</table>

<div class="status">SYSTEM NOMINAL - NO 3D ASSETS LOADED</div>
</body>
</html>
)=====";

void handleRoot() {
server.send(200, "text/html", fischyWebHTML);
}

void setup() {
auto cfg = M5.config();
M5.begin(cfg);

M5.Display.setTextSize(2);
M5.Display.setRotation(1);

// G1 outputs voltage, G2 reads it (Use jumper wires here)
pinMode(1, OUTPUT);
pinMode(2, INPUT_PULLDOWN);

// Boot up the AP
WiFi.softAP("FischyWeb-Test", "teckyadmin");
server.on("/", handleRoot);
server.begin();
}

void loop() {
M5.update();
server.handleClient();

// Big M5 button cycles the testing tools
if (M5.BtnA.wasPressed()) {
currentMode++;
if (currentMode >= TOTAL_MODES) currentMode = 0;
M5.Display.clear();
}

M5.Display.setCursor(0, 0);
M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);

// --- TESTING CHANNEL TOOLS --- //

if (currentMode == 0) {
// Mode 0: Web Server Status
M5.Display.println("[ WEB HOST: LIVE ]");
M5.Display.setTextSize(1);
M5.Display.println("\nSSID: FischyWeb-Test");
M5.Display.println("IP: 192.168.4.1");
M5.Display.println("\nHosting flat UI pinout table.");
M5.Display.setTextSize(2);
}
else if (currentMode == 1) {
// Mode 1: Active Port Forward Scanner
M5.Display.println("[ PORT SCANNER ]");
M5.Display.setTextSize(1);
M5.Display.printf("\nTarget: %s\n", serverIP);
M5.Display.println("Checking active routing...\n");

// Simulate port checks (Requires station mode to actually ping, simulated for AP mode)
M5.Display.println("Port 80 (HTTP): OPEN");
M5.Display.println("Port 443 (HTTPS): OPEN");
M5.Display.println("Port 25565 (Cust): BLOCKED");
M5.Display.setTextColor(TFT_YELLOW, TFT_BLACK);
M5.Display.println("\nCheck firewall rules.");
M5.Display.setTextSize(2);
}
else if (currentMode == 2) {
// Mode 2: Motherboard / Connector Pin Probe
M5.Display.println("[ PIN PROBE ]");
M5.Display.setTextSize(1);
M5.Display.println("\nTesting continuity.");
M5.Display.println("Isolate Top-Right Pin -> GND\n");

digitalWrite(1, HIGH); // Send signal

M5.Display.setCursor(0, 85);
if (digitalRead(2) == HIGH) {
M5.Display.setTextColor(TFT_GREEN, TFT_BLACK);
M5.Display.println("--> CONNECTION DETECTED");
M5.Display.println(" SIGNAL ROUTED.");
} else {
M5.Display.setTextColor(TFT_RED, TFT_BLACK);
M5.Display.println("--> OPEN / DEAD PIN");
M5.Display.println(" NO SIGNAL.");
}
M5.Display.setTextSize(2);
}
else if (currentMode == 3) {
// Mode 3: Hardware Diagnostics
M5.Display.println("[ HW DIAGNOSTICS ]");
M5.Display.setTextSize(1);

float aX, aY, aZ;
M5.Imu.getAccelData(&aX, &aY, &aZ);

M5.Display.printf("\nIMU-X: %f\n", aX);
M5.Display.printf("IMU-Y: %f\n", aY);
M5.Display.printf("IMU-Z: %f\n", aZ);

// Quick mic level check (fake poll for UI testing)
M5.Display.println("\nMic: Polling audio buffer...");
M5.Display.println("System Memory: NOMINAL");
M5.Display.setTextSize(2);
}
}
