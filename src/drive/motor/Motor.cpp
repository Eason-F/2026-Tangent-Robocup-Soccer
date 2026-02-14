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

    setMotorSpeed(0);
}

void Motor::incrementPulseCount() {
    pulseCount++;
}

void Motor::setMotorSpeed(int speed) {
    int motorSpeed = 255 - abs((speed / 100.0) * 255);
    digitalWrite(directionPin, (speed > 0) ? LOW : HIGH);
    analogWrite(pwmPin, motorSpeed);
}
