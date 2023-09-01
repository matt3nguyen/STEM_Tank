#include <Servo.h>

Servo doorServo1;
Servo doorServo2;

#define sensorDoor1 4
#define sensorDoor2 3

int potpin = 0; // Analog pin used to connect the potentiometer
int val;        // Variable to read the value from the analog pin

void setup()
{
  pinMode(sensorDoor1, INPUT);
  doorServo1.attach(8);
  doorServo2.attach(10);
}

void loop()
{
  // if object is detecting coming to box 1. Box 1 door opens
  if (digitalRead(sensorDoor1) == LOW)
  {
    doorServo1.write(180);
    delay(2000);
    // if object is detecting coming to box 2. Box 2 door opens
  }
  else if (digitalRead(sensorDoor2) == LOW)
  {
    doorServo2.write(180);
    delay(2000);
    // if nothing is detected, the door remained shut
  }
  else
  {

    doorServo1.write(0);
    doorServo2.write(0);
  }
}
