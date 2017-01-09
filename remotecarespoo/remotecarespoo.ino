/* YourDuino.com Example Software Sketch
  IR Remote Kit Test
  Uses YourDuino.com IR Infrared Remote Control Kit 2
  http://arduino-direct.com/sunshop/index.php?l=product_detail&p=153
  based on code by Ken Shirriff - http://arcfn.com
  Get Library at: https://github.com/shirriff/Arduino-IRremote
  Unzip folder into Libraries. RENAME folder IRremote
  terry@yourduino.com */

/*-----( Import needed libraries )-----*/

#include "IRremote.h"

/*-----( Declare Constants )-----*/
int receiver = 2; // pin 1 of IR receiver to Arduino digital pin 11

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'
/*-----( Declare Variables )-----*/

//motors: A is left, B is right
int enableA = 11;
int pinA1 = 5;
int pinA2 = 6;

int enableB = 10;
int pinB1 = 4;
int pinB2 = 3;

//ledPin
int ledPin = 13;

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
}/*--(end setup )---*/


void loop() {  /*----( LOOP: RUNS CONSTANTLY )----*/

  if (irrecv.decode(&results)) { // have we received an IR signal?
    Serial.println(results.value, HEX); // UN Comment to see raw values
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
//      Serial.println("GOING TURNING LEFT");
    } else if (dir == "right") {
      enableMotors();
      turnRight(200);
      disableMotors();
//      Serial.println("TURNING RIGHT");
    }else if(dir=="stop"){
      disableMotors();
    }
    irrecv.resume(); // receive the next value
  }
}/* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/
String translateIR() { // takes action based on IR code received

  // describing Car MP3 IR codes for LG remote arrows



  switch (results.value) {
    case 0x20DF02FD:
    case 0x63D1BF9F:
      Serial.println(" forward              ");
      return ("forward");
      break;

    case 0x20DFE01F:
    case 0x7C6035BF:
      Serial.println(" left              ");
      return ("left");
      break;

    case 0x8B7AFB3B:
    case 0x20DF609F:
      Serial.println(" right              ");
      return ("right");
      break;

    case 0xBD7F17A3:
    case 0x20DF827D:
      Serial.println(" back              ");
      return ("backward");
      break;
      
    case 0xDE8D48A3:
    case 0x20DF22DD:
      Serial.println("stop");
      return("stop");
      break;

     default: return "not regocnized";

  }

  delay(500);


} //END translateIR


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
