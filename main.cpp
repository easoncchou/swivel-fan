#include <Arduino.h>
#include <L293D.h>
#include <Servo.h>

// function prototypes
bool pollButton(int pin);
void pollFanSpeed();
void pollSwivel();
void swivel();

// declare variables
const int pinBlueButton = 4;
const int pinWhiteButton = 2;
const int pinServo =  3;
const int motorPinA = 9;
const int motorPinB = 10;
const int motorPinEnable = 11;
Servo fanServo;
L293D motor(motorPinA, motorPinB, motorPinEnable);
bool isSwiveling = false;
int swivelPos = 0;
int swivelDelta = 1;
int fanSpeed = 0;

void setup() {
  pinMode(pinBlueButton, INPUT);
  pinMode(pinWhiteButton, INPUT);
  fanServo.attach(pinServo);
  fanServo.write(0); // reset servo position
  motor.begin(true);
  motor.SetMotorSpeed(0);
}

void loop() {
  swivel();
  pollSwivel();
  pollFanSpeed();
}

bool pollButton(int pin) {
  int buttonState = digitalRead(pin);
  if (buttonState == HIGH) {
    while (buttonState == HIGH) {
      buttonState = digitalRead(pin);
    }
    return true;
  } else {
    return false;
  }
}

void pollFanSpeed() {
  if (pollButton(pinWhiteButton) == false) return;
  if (fanSpeed == 2) {
    fanSpeed = 0;
  } else {
    fanSpeed++;
  }
  motor.SetMotorSpeed(fanSpeed * 40);
}

void pollSwivel() {
  if(pollButton(pinBlueButton)) {
    isSwiveling = !isSwiveling;
  }
}

void swivel() {
  if (isSwiveling) {
    if (swivelPos > 160 || swivelPos < 0) {
      swivelDelta *= -1;
    }
    swivelPos += swivelDelta;
    fanServo.write(swivelPos);
    delay(25);
  }
}
