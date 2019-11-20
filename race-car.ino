#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define leftForward 0
#define leftBackward 1
#define rightForward 2
#define rightBackward 3

const int frequency = 5000;
const int resolution = 8;
const int fullSpeedValue = 255; // 0-255
const int slowSpeedValue = 150; // 0-255
const int stopSpeedValue = 0; // 0-255

const int leftWheelsSwitch = 33;
const int leftWheelsDirection = 32;
const int rightWheelsSwitch = 23;
const int rightWheelsDirection = 22;

WebServer server(80);

void handleHome() {
  server.send(200, "text/html", "<a href='/go'>Forward</a><br/><a href='/back'>Backward</a><br/><a href='/stop'>Stop</a><br/>");
}

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void moveForward() {
  ledcWrite(leftBackward, stopSpeedValue);
  ledcWrite(rightBackward, stopSpeedValue);
  ledcWrite(leftForward, fullSpeedValue);
  ledcWrite(rightForward, fullSpeedValue);
  server.send(200, "text/plain", "Go");
}

void moveBackward() {
  ledcWrite(leftForward, stopSpeedValue);
  ledcWrite(rightForward, stopSpeedValue);
  ledcWrite(leftBackward, fullSpeedValue);
  ledcWrite(rightBackward, fullSpeedValue);
  server.send(200, "text/plain", "Back");
}

void brake() {
  ledcWrite(leftForward, stopSpeedValue);
  ledcWrite(rightForward, stopSpeedValue);
  ledcWrite(leftBackward, stopSpeedValue);
  ledcWrite(rightBackward, stopSpeedValue);
  server.send(200, "text/plain", "Stop");
}

void setup(void) {
  ledcSetup(leftForward, frequency, resolution);
  // left
  pinMode(leftWheelsSwitch, OUTPUT);
  pinMode(leftWheelsDirection, OUTPUT);
  ledcAttachPin(leftWheelsSwitch, leftForward);
  ledcAttachPin(leftWheelsDirection, leftBackward);
  // right
  pinMode(rightWheelsSwitch, OUTPUT);
  pinMode(rightWheelsDirection, OUTPUT);
  ledcAttachPin(rightWheelsSwitch, rightForward);
  ledcAttachPin(rightWheelsDirection, rightBackward);
  
  Serial.begin(115200);
  WiFi.softAP("Zuhlke Asiaaaa Car", "Not1Less");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", handleHome);
  server.on("/go", moveForward);
  server.on("/back", moveBackward);
  server.on("/stop", brake);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
