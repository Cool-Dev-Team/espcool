#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32-GPIO";
const char* password = "12345678";

WebServer server(80);

// Safe GPIOs for ESP32-S3 DevKitC-1 (avoid strapping pins 0,3,4,5,6,7,8,9,10,11,12,13?)
const int pins[] = {14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39};
const int pinCount = sizeof(pins)/sizeof(pins[0]);

String getPage() {
  String html = "<html><head><meta http-equiv='refresh' content='2'><style>table{width:100%;border-collapse:collapse}th,td{border:1px solid #ddd;padding:8px}th{background:#f2f2f2}.high{background:#90EE90}.low{background:#FFCCCB}</style></head><body>";
  html += "<h1>GPIO States</h1><p>IP: " + WiFi.softAPIP().toString() + "</p><table><tr><th>GPIO</th><th>State</th></tr>";
  for(int i=0; i<pinCount; i++) {
    int state = digitalRead(pins[i]);
    html += "<tr class='" + String(state?"high":"low") + "'><td>" + String(pins[i]) + "</td><td>" + String(state?"HIGH":"LOW") + "</td></tr>";
  }
  html += "</table></body></html>";
  return html;
}

void setup() {
  Serial.begin(115200);
  for(int p : pins) pinMode(p, INPUT);
  WiFi.softAP(ssid, password);
  server.on("/", [](){ server.send(200, "text/html", getPage()); });
  server.begin();
  Serial.println("Ready: " + WiFi.softAPIP().toString());
}

void loop() { server.handleClient(); }
