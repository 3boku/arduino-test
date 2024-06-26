#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <Servo.h>

// Motor definitions
AF_DCMotor motor_L(1);
AF_DCMotor motor_R(4);

// Bluetooth pins
#define BT_RXD A5
#define BT_TXD A4

// Ultrasonic sensor pins
int TrigPin = A0;
int EchoPin = A1;
long duration, distance;

// LED pins
int RedL = A2;
int GreenL = A3;

// Motor speeds
int Lspeed = 250;
int Rspeed = 250;

// Bluetooth setup
SoftwareSerial BTSerial(BT_TXD, BT_RXD);

// Mode
bool autoMode = false;

void setup() {
  // Initialize motors
  motor_L.setSpeed(0);
  motor_L.run(RELEASE);
  motor_R.setSpeed(0);
  motor_R.run(RELEASE);

  // Initialize pins
  pinMode(EchoPin, INPUT);
  pinMode(TrigPin, OUTPUT);

  // Initialize serial communication
  BTSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  if (BTSerial.available()) {
    char val = BTSerial.read();
    if (val == 'f') {
      autoMode = false;
      analogWrite(GreenL, 0);
      analogWrite(RedL, 0);
      Forward();
    } else if (val == 'b') {
      autoMode = false;
      analogWrite(GreenL, 0);
      analogWrite(RedL, 0);
      Backward();
    } else if (val == 'r') {
      autoMode = false;
      analogWrite(GreenL, 0);
      analogWrite(RedL, 0);
      Right();
    } else if (val == 'l') {
      autoMode = false;
      analogWrite(GreenL, 0);
      analogWrite(RedL, 0);
      Left();
    } else if (val == 's') {
      autoMode = false;
      analogWrite(GreenL, 0);
      analogWrite(RedL, 0);
      Stop();
    } else if (val == 'a') {
      autoMode = true;
    }
  }

  if (autoMode) {
    Forward();
    analogWrite(GreenL, 255);
    analogWrite(RedL, 0);
    delay(50);
    Obstacle_Check();
  }
}
void Distance_Measurement() {
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  duration = pulseIn(EchoPin, HIGH);
  distance = ((float)(340 * duration) / 1000) / 2;
  delay(50);
}

void Obstacle_Check() {
  int val = random(2);
  Distance_Measurement();
  delay(50);  
  Serial.println(distance);
  while (distance < 300) {
    analogWrite(GreenL, 0);
    analogWrite(RedL, 255);
    if (distance < 150) {
      Backward();
      delay(800);
      Stop();
      delay(50);
      Distance_Measurement();
      delay(100);   
    } else {
      if (val == 0) {
        Right();
        delay(400);
      } else if (val == 1) {
        Left();
        delay(400);
      }
      Distance_Measurement();
      delay(100);
    }
  }
}

void Forward() {
  motor_L.run(FORWARD);
  motor_R.run(FORWARD);
  motor_L.setSpeed(Lspeed);
  motor_R.setSpeed(Rspeed);
}

void Backward() {
  motor_L.run(BACKWARD);
  motor_R.run(BACKWARD);
  motor_L.setSpeed(Lspeed);
  motor_R.setSpeed(Rspeed);
}

void Right() {
  motor_L.run(FORWARD);
  motor_R.run(BACKWARD);
  motor_L.setSpeed(Lspeed);
  motor_R.setSpeed(Rspeed * 0.5);
}

void Left() {
  motor_L.run(BACKWARD);
  motor_R.run(FORWARD);
  motor_L.setSpeed(Lspeed * 0.5);
  motor_R.setSpeed(Rspeed);
}

void Stop() {
  motor_L.run(RELEASE);
  motor_R.run(RELEASE);
  motor_L.setSpeed(0);
  motor_R.setSpeed(0);
}
