#include <Servo.h>
Servo myservo;  // Create servo object to control a servo

// Relay pins
const int RELAY_PIN2 = 2;  
const int RELAY_PIN3 = 3;
const int RELAY_PIN4 = 4;
const int RELAY_PIN5 = 5;

// Sensor pins
#define sensor1 7
#define sensor2 8

// Ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

void setup() {
  // Initialize pins
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  pinMode(RELAY_PIN4, OUTPUT);
  pinMode(RELAY_PIN5, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
  
  // Initialize servo
  myservo.attach(11);
  myservo.write(90);
  
  // Initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Ultrasonic sensor distance measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  // Check if distance is within certain range and sensor1 is active
  if (distance <= 3 && digitalRead(sensor1) == HIGH) {
    //check to see what product is being put on the conveyer belts to sort it accordingly
    //the servo control the devider
    myservo.write(0);
    digitalWrite(RELAY_PIN2, LOW);
    delay(2000);
    digitalWrite(RELAY_PIN3, LOW);
    delay(5000);
  }
  // Check if distance is within certain range and sensor1 is inactive
  else if (distance <= 3 && digitalRead(sensor1) == LOW) {
    // devider swich on to direct product to a different pick up box
    delay(1000);
    myservo.write(70);
    digitalWrite(RELAY_PIN3, HIGH);
    digitalWrite(RELAY_PIN2, LOW);
    Serial.println("white");
    delay(2000);
  }
  else {
    //keep the devider at its original position
    digitalWrite(RELAY_PIN2, HIGH);
    digitalWrite(RELAY_PIN3, HIGH);
    myservo.write(0);
    Serial.println("black");
  }

  // Check if sensor2 is active
  if (digitalRead(sensor2) == LOW) {
    // Turn on relay to run the conveyerbelts
    digitalWrite(RELAY_PIN4, LOW);
    delay(2000);
  } else {
    // Turn off when the food reach its destination
    digitalWrite(RELAY_PIN4, HIGH);
  }
}
