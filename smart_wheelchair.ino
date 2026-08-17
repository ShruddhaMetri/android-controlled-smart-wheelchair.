/*
 * Android-Controlled Smart Wheelchair
 * -----------------------------------
 * Platform : Arduino Uno (ATmega328P)
 * Control  : HC-05 Bluetooth module, driven from an Android app
 * Drive    : L298N dual H-bridge motor driver (left + right motors)
 * Safety   : HC-SR04 ultrasonic sensor + buzzer for obstacle alert
 *
 * Bluetooth commands (single characters sent by the Android app):
 *   'F' = forward   'B' = backward
 *   'L' = left      'R' = right
 *   'S' = stop
 *
 * Obstacle safety:
 *   If an obstacle is closer than OBSTACLE_CM while moving forward,
 *   the wheelchair stops and the buzzer sounds.
 *
 * Pin mapping:
 *   HC-05 Bluetooth : RX=2, TX=3 (SoftwareSerial)
 *   Buzzer          : 4
 *   Left motor  (L298N) : ENA=5 (PWM), IN1=6, IN2=7
 *   Right motor (L298N) : ENB=10 (PWM), IN3=8, IN4=9
 *   Ultrasonic HC-SR04  : TRIG=12, ECHO=13
 */

#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2, 3);  // RX, TX

// ---- Buzzer ----
const int BUZZER = 4;

// ---- Motor driver (L298N) ----
const int ENA = 5,  IN1 = 6, IN2 = 7;   // left motor
const int ENB = 10, IN3 = 8, IN4 = 9;   // right motor

// ---- Ultrasonic ----
const int TRIG = 12, ECHO = 13;

// ---- Settings ----
const int MOTOR_SPEED = 200;   // 0-255 PWM
const int OBSTACLE_CM = 30;    // stop-forward distance in cm

char command = 'S';

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  pinMode(BUZZER, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);

  stopMotors();
}

void loop() {
  // read a command if the app sent one
  if (bluetooth.available()) {
    command = bluetooth.read();
  }

  long distance = readDistanceCm();

  // safety: block forward motion if something is too close
  if (command == 'F' && distance > 0 && distance < OBSTACLE_CM) {
    stopMotors();
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
    executeCommand(command);
  }

  delay(50);
}

// ---- Command handling ----
void executeCommand(char c) {
  switch (c) {
    case 'F': forward();  break;
    case 'B': backward(); break;
    case 'L': left();     break;
    case 'R': right();    break;
    case 'S':
    default:  stopMotors(); break;
  }
}

// ---- Ultrasonic distance in cm ----
long readDistanceCm() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);  // timeout ~5 m
  if (duration == 0) return -1;                // no echo received
  return duration * 0.034 / 2;
}

// ---- Motor primitives ----
void setLeft(bool fwd) {
  digitalWrite(IN1, fwd ? HIGH : LOW);
  digitalWrite(IN2, fwd ? LOW : HIGH);
  analogWrite(ENA, MOTOR_SPEED);
}

void setRight(bool fwd) {
  digitalWrite(IN3, fwd ? HIGH : LOW);
  digitalWrite(IN4, fwd ? LOW : HIGH);
  analogWrite(ENB, MOTOR_SPEED);
}

void forward()  { setLeft(true);  setRight(true);  }
void backward() { setLeft(false); setRight(false); }
void left()     { setLeft(false); setRight(true);  }
void right()    { setLeft(true);  setRight(false); }

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}
