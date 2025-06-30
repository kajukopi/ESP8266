#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

// Replace these with your WiFi credentials
const char* ssid = "karimroy";
const char* password = "09871234";

// Create web server on port 80
ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>Hello from NodeMCU</h1><p>OTA Ready</p>");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  // Set up web server routes
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  // OTA Setup
  ArduinoOTA.setHostname("NodeMCU-OTA");
  ArduinoOTA.begin();
  Serial.println("OTA Ready");
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
}