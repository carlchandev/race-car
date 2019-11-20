#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <mutex>
using namespace std;

#define leftForward 0
#define leftBackward 1
#define rightForward 2
#define rightBackward 3

mutex m;

const int frequency = 5000;
const int resolution = 8;
const int fullSpeedValue = 255; // 0-255
const int slowSpeedValue = 180; // 0-255
const int stopSpeedValue = 0; // 0-255

const int leftWheelsSwitch = 33;
const int leftWheelsDirection = 32;
const int rightWheelsSwitch = 23;
const int rightWheelsDirection = 22;

WebServer server(80);

void handleHome() {
  server.send(200, "text/html", "<a href='/go'>Forward</a><br/><a href='/back'>Backward</a><br/><a href='/stop'>Stop</a><br/><a href='/left'>Left</a><br/><a href='/right'>Right</a><br/>");
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
  m.lock();
  ledcWrite(leftBackward, stopSpeedValue);
  ledcWrite(rightBackward, stopSpeedValue);
  ledcWrite(leftForward, fullSpeedValue);
  ledcWrite(rightForward, fullSpeedValue);
  m.unlock();
  server.send(200, "text/plain", "Go");
}

void moveForwardSlow() {
  m.lock();
  ledcWrite(leftBackward, stopSpeedValue);
  ledcWrite(rightBackward, stopSpeedValue);
  ledcWrite(leftForward, slowSpeedValue);
  ledcWrite(rightForward, slowSpeedValue);
  m.unlock();
  server.send(200, "text/plain", "Go Slow");
}

void moveBackward() {
  m.lock();
  ledcWrite(leftForward, stopSpeedValue);
  ledcWrite(rightForward, stopSpeedValue);
  ledcWrite(leftBackward, fullSpeedValue);
  ledcWrite(rightBackward, fullSpeedValue);
  m.unlock();
  server.send(200, "text/plain", "Back");
}

void moveBackwardSlow() {
  m.lock();
  ledcWrite(leftForward, stopSpeedValue);
  ledcWrite(rightForward, stopSpeedValue);
  ledcWrite(leftBackward, slowSpeedValue);
  ledcWrite(rightBackward, slowSpeedValue);
  m.unlock();
  server.send(200, "text/plain", "Back Slow");
}

void moveLeft() {
  m.lock();
  ledcWrite(leftForward, stopSpeedValue);
  ledcWrite(rightForward, fullSpeedValue);
  ledcWrite(leftBackward, stopSpeedValue);
  ledcWrite(rightBackward, stopSpeedValue);
  server.send(200, "text/plain", "Left");
  m.unlock();
}

void moveRight() {
  m.lock();
  ledcWrite(leftForward, fullSpeedValue);
  ledcWrite(rightForward, stopSpeedValue);
  ledcWrite(leftBackward, stopSpeedValue);
  ledcWrite(rightBackward, stopSpeedValue);
  m.unlock();
  server.send(200, "text/plain", "Right");
  
}

void spin() {
  m.lock();
  ledcWrite(leftForward, fullSpeedValue);
  ledcWrite(rightForward, stopSpeedValue);
  ledcWrite(leftBackward, stopSpeedValue);
  ledcWrite(rightBackward, fullSpeedValue);
  m.unlock();
  server.send(200, "text/plain", "Stop");
}

void brake() {
  m.lock();
  ledcWrite(leftForward, stopSpeedValue);
  ledcWrite(rightForward, stopSpeedValue);
  ledcWrite(leftBackward, stopSpeedValue);
  ledcWrite(rightBackward, stopSpeedValue);
  m.unlock();
  server.send(200, "text/plain", "Stop");
}

void setup(void) {
  ledcSetup(leftForward, frequency, resolution);
  ledcSetup(leftBackward, frequency, resolution);
  ledcSetup(rightForward, frequency, resolution);
  ledcSetup(rightBackward, frequency, resolution);
  
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
  server.on("/goSlow", moveForwardSlow);
  server.on("/back", moveBackward);
  server.on("/backSlow", moveBackwardSlow);
  server.on("/left", moveLeft);
  server.on("/right", moveRight);
  server.on("/spin", spin);
  server.on("/stop", brake);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  // ready LED lights up
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop(void) {
  server.handleClient();
}
