#include <Arduino.h>

#include <drive/motor/Motor.hpp>

Motor::Motor() {
    this-> pwmPin = 99;
    this-> directionPin = 99;
    this-> pulsePin = 99;
}

Motor::Motor(const int &pwmPin, const int &directionPin, const int &pulsePin) {
    this-> pwmPin = pwmPin;
    this-> directionPin = directionPin;
    this-> pulsePin = pulsePin;

    pinMode(pwmPin, OUTPUT);
    pinMode(directionPin, OUTPUT);
    pinMode(pulsePin, INPUT);
}

void Motor::incrementPulseCount() {
    pulseCount++;
}

void Motor::setMotorSpeed(int speed) {
    speed = 255 - (speed / 100) * 255;
    digitalWrite(directionPin, (speed > 0) ? HIGH : LOW);
    analogWrite(pwmPin, speed);
}
