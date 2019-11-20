#include <WiFi.h>
#include <Hash.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#define leftForward 0
#define leftBackward 1
#define rightForward 2
#define rightBackward 3

const int frequency = 5000;
const int resolution = 8;
const int fullSpeedValue = 255; // 0-255
const int slowSpeedValue = 150; // 0-255
const int stopSpeedValue = 0; // 0-255

// the setup function runs once when you press reset or power the board
const int leftWheelsSwitch = 33;
const int leftWheelsDirection = 32;
const int rightWheelsSwitch = 23;
const int rightWheelsDirection = 22;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });

  AsyncElegantOTA.begin(server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");

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
}

void brake() {
  ledcWrite(leftForward, stopSpeedValue);
  ledcWrite(rightForward, stopSpeedValue);
  ledcWrite(leftBackward, stopSpeedValue);
  ledcWrite(rightBackward, stopSpeedValue);
  delay(1000);
}

void moveForward() {
  ledcWrite(leftBackward, stopSpeedValue);
  ledcWrite(rightBackward, stopSpeedValue);
  ledcWrite(leftForward, fullSpeedValue);
  ledcWrite(rightForward, fullSpeedValue);
  delay(3000);
}

void moveBackward() {
  ledcWrite(leftForward, stopSpeedValue);
  ledcWrite(rightForward, stopSpeedValue);
  ledcWrite(leftBackward, fullSpeedValue);
  ledcWrite(rightBackward, fullSpeedValue);
  delay(3000);
}

void loop() {
  AsyncElegantOTA.loop();
// moveForward();
// moveBackward();
   brake();
}
