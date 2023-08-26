// Written By Rene Karkkainen - r3ne.net

#include "Arduino.h"
#include "uRTCLib.h"

// uRTCLib rtc;
uRTCLib rtc(0x68);

// Define motor pins
const int motorPins[] = { 2, 3, 4, 5 };  // IN1, IN2, IN3, IN4

// Display led under clock numbers
const int display_Led = 6;

// Power led to come on always when relay is closed.
const int power_Led = 10;
// Switch from auto to ever-on
const int off_switch = 9;

// Buttons inside the clock
// Button that goes on when sleep timer is set.
const int sleep_timer_button = 8;

// Button that goes on when alarm should sound.
const int alarm_button = 7;

// Set this to your relay pin.
int relay_module = 12;


// States of the buttons.
int off_state = 0;
int sleep_state = 0;
int alarm_state = 0;

// Ignore this. It is for my remote cotrolled relay.
int relay_pin = 11;
int relay_state = 0;

// Define the step sequence for the motor
const int stepSequence[][4] = {
  { 1, 0, 0, 0 },
  { 0, 1, 0, 0 },
  { 0, 0, 1, 0 },
  { 0, 0, 0, 1 },
  { 0, 0, 0, 0 },
};

// Define the total number of steps per revolution
const int stepsPerRevolution = 4096;

// Function to control the motor by steps
void move(int numSteps) {
  for (int i = 0; i < numSteps; i++) {
    for (int j = 0; j < 5; j++) {
      for (int k = 0; k < 4; k++) {
        digitalWrite(motorPins[k], stepSequence[j][k]);
      }
      unsigned long start;
      start = millis();
      while (start + 20 > millis()) {
        func();
      }
    }
  }
}

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }

  pinMode(display_Led, OUTPUT);
  pinMode(power_Led, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  pinMode(relay_module, OUTPUT);
  digitalWrite(display_Led, HIGH);
  digitalWrite(power_Led, HIGH);

  pinMode(off_switch, INPUT_PULLUP);
  pinMode(sleep_timer_button, INPUT_PULLUP);
  pinMode(alarm_button, INPUT_PULLUP);

  Serial.begin(9600);
  delay(3000);  // wait for console opening

  URTCLIB_WIRE.begin();

  rtc.set(0, 56, 12, 5, 13, 1, 22);
  Serial.println("More info: https://github.com/JAAKKQ/PP-215");
}

void func() {
  rtc.refresh();

  off_state = digitalRead(off_switch);
  sleep_state = digitalRead(sleep_timer_button);
  alarm_state = digitalRead(alarm_button);

  if (off_state == 1 || sleep_state == 1 || alarm_state == 1) {
    // Close relay
    digitalWrite(power_Led, HIGH);
    relay(true);
  } else {
    digitalWrite(power_Led, LOW);
    relay(false);
  }
}

void relay(bool state) {
  if (state) {
    if (relay_state == 0) {
      digitalWrite(relay_module, HIGH);
      digitalWrite(relay_pin, HIGH);
      delay(50);
      digitalWrite(relay_pin, LOW);
      relay_state = 1;
    }
  } else {
    if (relay_state == 1) {
      digitalWrite(relay_module, LOW);
      digitalWrite(relay_pin, HIGH);
      delay(50);
      digitalWrite(relay_pin, LOW);
      delay(100);
      digitalWrite(relay_pin, HIGH);
      delay(50);
      digitalWrite(relay_pin, LOW);
      relay_state = 0;
    }
  }
}

void loop() {
  func();
  if (rtc.second() == 0) {
    move(41);
  }
}