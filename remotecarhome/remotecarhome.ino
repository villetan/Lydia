/* Lydia, 4wd Arduino robot. Constantly imporivng its actions */

/*-----( Import needed libraries )-----*/
#include <NewPing.h>
#include <Servo.h>
#include "IRremote.h"

/*-----( Declare Constants )-----*/
const int receiver = 2; 

//ledPin
const int ledPin = 13;

//servoPin
const int servoPin = 12;

//sensor
const int trigPin = 7; // harmaa
const int echoPin = 8; // valkonen

//motors: A is left, B is right
const int enableA = 11;
const int pinA1 = 5;
const int pinA2 = 6;

const int enableB = 10;
const int pinB1 = 4;
const int pinB2 = 3;
/*-----( Declare objects )-----*/
Servo Servo1;
NewPing sonar(trigPin, echoPin, 400);
IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'
/*-----( Declare Variables )-----*/




void setup()   /*----( SETUP: RUNS ONCE )----*/
{

  pinMode(ledPin, OUTPUT);

  pinMode(enableA, OUTPUT);
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);

  pinMode(enableB, OUTPUT);
  pinMode(pinB1, OUTPUT);
  pinMode(pinB2, OUTPUT);
  Serial.begin(9600);
  Serial.println("IR Receiver Raw Data + Button Decode Test");
  irrecv.enableIRIn(); // Start the receiver
  enableMotors();
  Servo1.attach(servoPin);
}/*--(end setup )---*/


void loop() {  /*----( LOOP: RUNS CONSTANTLY )----*/
  if (irrecv.decode(&results)) { // have we received an IR signal?
    Serial.println(results.value, HEX); //see the raw values
    String dir = translateIR();
    if (dir == "forward" || dir == "backward") {
      enableMotors();
      if (dir == "forward") {
        forward();
      } else if (dir == "backward") {
        backward();
      }
    }
    else if (dir == "left") {
      enableMotors();
      turnLeft(200);
      disableMotors();
      //      Serial.println("TURNING LEFT");
    } else if (dir == "right") {
      enableMotors();
      turnRight(200);
      disableMotors();
      //      Serial.println("TURNING RIGHT");
    } else if (dir=="auto"){
      irrecv.resume();
      autopilot();
    } else if (dir == "stop") {
      disableMotors();
    }
    irrecv.resume(); // receive the next value
  }
}/* --(end main loop )-- */


String translateIR() { // takes action based on IR code received
  // describing  IR codes for SAMSUNG TV controller
  switch (results.value) {
    case 0xE0E006F9:
    case 0xC26BF044:
      Serial.println(" forward");
      return ("forward");
      break;
      
    case 0xE0E0A659:
    case 0x758C9D82:
      Serial.println(" left");
      return ("left");
      break;

    case 0xE0E046B9:
    case 0x53801EE8:
      Serial.println(" right");
      return ("right");
      break;

    case 0xE0E08679:
    case 0xC4FFB646:
      Serial.println(" back");
      return ("backward");
      break;

    case 0xE0E016E9:
    case 0xE3105407:
      Serial.println("stop");
      return ("stop");
      break;

    case 0xE0E0E21D:
      Serial.println("AUTO-PILOT");
      return("auto");
    default: return "not regocnized";

  }

  delay(500);


}

void autopilot(){
  enableMotors();
  while(true){

    if (irrecv.decode(&results)) {
      if (results.value == 2249845039 || results.value == 3772801693 ){
        Serial.println("AUTO-PILOT OFF");
        coast(100);
        break;
      }
      irrecv.resume();
    }
    
    servoFront();
    long distance = getDistance();
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
   enableMotors();
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

//Distance controls
double getDistance() {
  delay(10);
  long distance = sonar.ping_cm();
  Serial.print(distance);
  Serial.println(" cm.");
  return distance;
}

//motor controls
void backward() {
  motorABackward();
  motorBBackward();
}

void forward() {
  motorAForward();
  motorBForward();
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

/* 
  IR Remote Kit Test
  Uses YourDuino.com IR Infrared Remote Control Kit 2
  http://arduino-direct.com/sunshop/index.php?l=product_detail&p=153
  based on code by Ken Shirriff - http://arcfn.com
  Get Library at: https://github.com/shirriff/Arduino-IRremote
  Unzip folder into Libraries. RENAME folder IRremote
  terry@yourduino.com */
