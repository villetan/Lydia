#include <NewPing.h>
#include <Servo.h>

//ledPin
int ledPin = 13;

//servoPin
int servoPin = 12;
Servo Servo1;

//sensor
int trigPin = 7; // harmaa
int echoPin = 8; // valkonen
NewPing sonar(trigPin, echoPin, 400);

//motors: A is left, B is right
int enableA = 11;
int pinA1 = 5;
int pinA2 = 6;

int enableB = 10;
int pinB1 = 4;
int pinB2 = 3;


long distance;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);

  pinMode(enableA, OUTPUT);
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);

  pinMode(enableB, OUTPUT);
  pinMode(pinB1, OUTPUT);
  pinMode(pinB2, OUTPUT);
  Serial.begin(9600);
  enableMotors();
  Servo1.attach(servoPin);

}

boolean run = true;
void loop() {
  // put your main code here, to run repeatedly:


  servoFront();
  distance = getDistance();
  digitalWrite(ledPin, LOW);
  forward();
  if (distance < 30 && distance > 0 ) {
    digitalWrite(ledPin, HIGH);
    Serial.write("JUMPING TO IF CLAUSE");
    backward(280);
    coast(300);
 

    servoLeft();
    delay(630);
    int leftDist = getDistance();
    Serial.print("left: ");
    Serial.println(leftDist);
    delay(20);

    servoRight();
    delay(830);
    int rightDist = getDistance();
    Serial.print("right: ");
    Serial.println(rightDist);
    delay(100);
    servoFront();

    Serial.print("MAX: ");
    Serial.println(max(leftDist, rightDist));
    coast(500);

    if (leftDist > rightDist) {
      Serial.println("TURNING LEFT");
      turnLeft(250);
      delay(100);
    } else if (rightDist > leftDist) {
      Serial.println("TURNING RIGHT");
      turnRight(250);
      delay(100);
    } else {
      backward(500);
      Serial.println("BACKING");
    }
    coast(500);
  }


}

//servo controls
void servoLeft() {
  Servo1.write(170);
}
void servoFront() {
  Servo1.write(80);
}
void servoRight() {
  Servo1.write(0);
}

double getDistance() {
  delay(10);
  distance = sonar.ping_cm();
  Serial.print(distance);
  Serial.println(" cm.");
  return distance;
}

void backward() {
  motorABackward();
  motorBBackward();
}

void forward() {
  motorAForward();
  motorBForward();
}

void testMotors() {
  if (run) {
    enableMotors();
    forward(2000);
    pauseBetween(1000);
    backward(2000);
    pauseBetween(1000);
    turnLeft(2000);
    pauseBetween(1000);
    turnRight(2000);
    disableMotors();
    run = false;
  }
}

void pauseBetween(int time) {
  disableMotors();
  delay(time);
  enableMotors();
}

void motorAOn() {
  digitalWrite(enableA, HIGH);
}

void motorBOn() {
  digitalWrite(enableB, HIGH);
}

void enableMotors() {
  motorAOn();
  motorBOn();
}

void motorAOff() {
  digitalWrite(enableA, LOW);
}

void motorBOff() {
  digitalWrite(enableB, LOW);
}

void disableMotors() {
  motorAOff();
  motorBOff();
}

//motor a controls
void motorAForward() {
  digitalWrite(pinA2, HIGH);
  digitalWrite(pinA1, LOW);
}

void motorABackward() {
  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, LOW);
}

//motor b controls

void motorBForward() {
  digitalWrite(pinB1, HIGH);
  digitalWrite(pinB2, LOW);
}

void motorBBackward() {
  digitalWrite(pinB2, HIGH);
  digitalWrite(pinB1, LOW);
}

//vehicle controls
void forward(int time) {
  motorAForward();
  motorBForward();
  delay(time);
}

void backward(int time) {
  motorABackward();
  motorBBackward();
  delay(time);
}



void turnLeft(int time) {
  motorABackward();
  motorBForward();
  delay(time);
}

void turnRight(int time) {
  motorBBackward();
  motorAForward();
  delay(time);
}

void coast(int time) {
  motorACoast();
  motorBCoast();
  delay(time);
}

void brake(int time) {
  motorABrake();
  motorBBrake();
  delay(time);
}

//coasting and breaking
void motorACoast() {
  digitalWrite(pinA1, LOW);
  digitalWrite(pinA2, LOW);
}
void motorABrake() {
  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, HIGH);
}

void motorBCoast() {
  digitalWrite(pinB1, LOW);
  digitalWrite(pinB2, LOW);
}

void motorBBrake() {
  digitalWrite(pinB1, HIGH);
  digitalWrite(pinB2, HIGH);
}


// for debugging we have light signals start fast blinker end slow blinker
void startSignal() {
  for (int i = 0; i < 10; ++i) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}

void between() {
  for (int i = 0; i < 5; ++i) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(1000);
  }
}

void endSignal() {
  for (int i = 0; i < 5; ++i) {
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}

