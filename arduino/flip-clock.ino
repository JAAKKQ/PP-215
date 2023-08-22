//Written By Rene Karkkainen - r3ne.net

#include "Arduino.h"
#include "uRTCLib.h"

// uRTCLib rtc;
uRTCLib rtc(0x68);

// Define pins for controlling the 28BYJ-48 motor
const int motorPins[] = { 2, 3, 4, 5 };  // IN1, IN2, IN3, IN4

// Define the step sequence for the motor
const int stepSequence[][4] = {
  { 1, 0, 0, 0 },
  { 1, 1, 0, 0 },
  { 0, 1, 0, 0 },
  { 0, 1, 1, 0 },
  { 0, 0, 1, 0 },
  { 0, 0, 1, 1 },
  { 0, 0, 0, 1 },
  { 0, 0, 0, 0 }
};

// Define the total number of steps per revolution
const int stepsPerRevolution = 4096;

// Function to control the motor by steps
void move(int numSteps) {
  for (int i = 0; i < numSteps; i++) {
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 4; k++) {
        digitalWrite(motorPins[k], stepSequence[j][k]);
      }
      unsigned long start;
      start = millis();
      while(start + 1 > millis()){}
    }
  }
}

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  Serial.begin(9600);
  delay(3000);  // wait for console opening

  URTCLIB_WIRE.begin();

  // Comment out below line once you set the date & time.
  // Following line sets the RTC with an explicit date & time
  // for example to set January 13 2022 at 12:56 you would call:
  rtc.set(0, 56, 12, 5, 13, 1, 22);
  // rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year)
  // set day of week (1=Sunday, 7=Saturday)
}

void loop() {
  rtc.refresh();

  Serial.print("Current Date & Time: ");
  Serial.print(rtc.year());
  Serial.print('/');
  Serial.print(rtc.month());
  Serial.print('/');
  Serial.print(rtc.day());

  Serial.print(" (");
  Serial.print(daysOfTheWeek[rtc.dayOfWeek() - 1]);
  Serial.print(") ");

  Serial.print(rtc.hour());
  Serial.print(':');
  Serial.print(rtc.minute());
  Serial.print(':');
  Serial.println(rtc.second());

  if (rtc.second() == 0) {
    move(200);
  };
  delay(1000);
}