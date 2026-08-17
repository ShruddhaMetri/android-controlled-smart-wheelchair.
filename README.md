# Android-Controlled Smart Wheelchair

A Bluetooth-controlled smart wheelchair driven from an Android app, with ultrasonic obstacle detection and buzzer alerts for safe navigation. Built on Arduino with embedded C firmware for real-time command parsing and motor control.

## Project Overview

The wheelchair receives movement commands from an Android app over a Bluetooth (HC-05) link. An Arduino parses each command and drives two motors through an L298N motor driver. A front-facing HC-SR04 ultrasonic sensor continuously checks for obstacles — if something is too close while moving forward, the wheelchair stops and a buzzer sounds.

## Features

- Wireless control from an Android app over Bluetooth
- Forward, backward, left, right, and stop movement
- Real-time ultrasonic obstacle detection
- Buzzer alert and automatic stop on obstacle
- Simple single-character command protocol

## System Architecture
[ Android App ] --Bluetooth--> [ HC-05 ] --> [ Arduino Uno ] --> [ L298N ] --> [ Motors ]
|
├── [ HC-SR04 Ultrasonic ]
└── [ Buzzer ]

## Bluetooth Command Set

| Command | Action |
|---------|--------|
| F | Move forward |
| B | Move backward |
| L | Turn left |
| R | Turn right |
| S | Stop |

## Pin Mapping

| Module | Arduino Pin |
|--------|-------------|
| HC-05 Bluetooth (RX, TX) | 2, 3 (SoftwareSerial) |
| Buzzer | 4 |
| Left motor (ENA, IN1, IN2) | 5, 6, 7 |
| Right motor (ENB, IN3, IN4) | 10, 8, 9 |
| HC-SR04 (TRIG, ECHO) | 12, 13 |

## Hardware Used

- Arduino Uno (ATmega328P)
- HC-05 Bluetooth module
- L298N dual H-bridge motor driver
- 2 DC geared motors
- HC-SR04 ultrasonic sensor
- Buzzer
- Battery pack, jumper wires, chassis

## Software

- Arduino IDE (sketch: `smart_wheelchair.ino`)
- Android Bluetooth controller app (any serial/RC controller app that sends the single-character commands above)

## How to Run

1. Open `smart_wheelchair.ino` in the Arduino IDE.
2. Wire the modules per the pin mapping above.
3. Select **Arduino Uno** and the correct COM port, then Upload.
4. Pair your phone with the HC-05 module (default PIN usually 1234 or 0000).
5. Open a Bluetooth controller app, connect to HC-05, and send F/B/L/R/S commands.

> Note: tune `MOTOR_SPEED` and `OBSTACLE_CM` for your chassis, and confirm the motor direction matches your wiring (swap IN pins if a motor runs backwards).

## Author

**Shruddha Metri**

Embedded Systems project — Arduino, Embedded C.
