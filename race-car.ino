/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

// the setup function runs once when you press reset or power the board
int leftWheelsSwitcvh = 33;
int rightWheelsSwitcvh = 23;

int fullSpeedPauseInterval = 0;
int slowSpeedPauseInterval = 30;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  // pinMode(LED_BUILTIN, OUTPUT);
}

void stopCar() {
  pinMode(leftWheelsSwitcvh, OUTPUT);
  digitalWrite(leftWheelsSwitcvh, LOW);    // turn the LED off by making the voltage LOW
  pinMode(rightWheelsSwitcvh, OUTPUT);
  digitalWrite(rightWheelsSwitcvh, LOW);  
}

// the loop function runs over and over again forever
void changeToSlowSpeed() {
  pinMode(leftWheelsSwitcvh, OUTPUT);
  digitalWrite(leftWheelsSwitcvh, HIGH);
  pinMode(rightWheelsSwitcvh, OUTPUT);
  digitalWrite(rightWheelsSwitcvh, HIGH);
  delay(5000);
}

void changeToFullSpeed() {
  pinMode(leftWheelsSwitcvh, OUTPUT);
  digitalWrite(leftWheelsSwitcvh, HIGH);
  pinMode(rightWheelsSwitcvh, OUTPUT);
  digitalWrite(rightWheelsSwitcvh, HIGH);
  delay(5000);
}

void loop() {
  changeToFullSpeed();
  delay(1000);
  changeToSlowSpeed();
  delay(1000);
}