#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FS.h> // SPIFFS

const char* ssid = "karimroy";
const char* password = "09871234";

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

const int ledPin = 2;

void handleLedOn() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "ON");
}

void handleLedOff() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/plain", "OFF");
}

void handleLedState() {
  server.send(200, "text/plain", digitalRead(ledPin) == LOW ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  SPIFFS.begin();

  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);
  server.on("/led/state", handleLedState);
  server.serveStatic("/", SPIFFS, "/index.html");

  httpUpdater.setup(&server); // /update

  server.begin();
  Serial.println("HTTP server started");

  ArduinoOTA.setHostname("NodeMCU-OTA");
  ArduinoOTA.begin();
  Serial.println("OTA Ready");
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
}

// Edit 2