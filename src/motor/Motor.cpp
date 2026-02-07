#include <motor/Motor.hpp>

Motor::Motor(const int pwmPin, const int directionPin, const int pulsePin) {
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

void Motor::setMotorSpeed(int speed, MotorDirection direction) {
    speed = 255 - (speed / 100) * 255;
    digitalWrite(directionPin, direction);
    analogWrite(pwmPin, speed);
}