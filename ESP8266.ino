#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>

const char* ssid = "karimroy";
const char* password = "09871234";

ESP8266WebServer server(80);
const int ledPin = D1;
bool ledState = false;

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP8266 LED Control</title>
  <style>
    body { font-family: Arial; text-align: center; padding: 50px; background: #f0f0f0; }
    h1 { color: #333; }
    button {
      padding: 15px 30px;
      margin: 10px;
      font-size: 18px;
      border: none;
      border-radius: 10px;
      cursor: pointer;
    }
    .on { background: #4CAF50; color: white; }
    .off { background: #f44336; color: white; }
    #status { font-size: 20px; margin-top: 20px; }
  </style>
</head>
<body>
  <h1>NodeMCU LED Control</h1>
  <button class="on" onclick="controlLED('on')">Turn ON</button>
  <button class="off" onclick="controlLED('off')">Turn OFF</button>
  <p id="status">LED Status: <span id="ledState">...</span></p>

  <script>
    function controlLED(state) {
      fetch('/' + state)
        .then(response => response.text())
        .then(text => document.getElementById('ledState').innerText = text);
    }

    setInterval(() => {
      fetch('/status')
        .then(res => res.text())
        .then(text => document.getElementById('ledState').innerText = text);
    }, 2000);
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleOn() {
  digitalWrite(ledPin, HIGH);
  ledState = true;
  server.send(200, "text/plain", "ON");
}

void handleOff() {
  digitalWrite(ledPin, LOW);
  ledState = false;
  server.send(200, "text/plain", "OFF");
}

void handleStatus() {
  server.send(200, "text/plain", ledState ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }

  Serial.println("\nWiFi connected: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/status", handleStatus);
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

// Edit File