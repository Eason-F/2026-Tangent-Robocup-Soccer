#include <Arduino.h>

#include <drive/motor/Motor.hpp>

Motor::Motor() {
    this-> pwmPin = 99;
    this-> directionPin1 = 99;
    this-> directionPin2 = 99;
}

Motor::Motor(const int &pwmPin, const int &directionPin1, const int &directionPin2) {
    this-> pwmPin = pwmPin;
    this-> directionPin1 = directionPin1;
    this-> directionPin2 = directionPin2;

    pinMode(pwmPin, OUTPUT);
    pinMode(directionPin1, OUTPUT);
    pinMode(directionPin2, OUTPUT);

    setMotorDutyCycle(0);
    brake();
};

Motor::Motor(const int &pwmPin, const int &directionPin1, const int &directionPin2,  const int &encoderPin1, const int &encoderPin2, PIDController pidController) {
    this-> pwmPin = pwmPin;
    this-> directionPin1 = directionPin1;
    this-> directionPin2 = directionPin2;
    this-> encoderPin1 = encoderPin1;
    this-> encoderPin2 = encoderPin2;
    this-> pidController = pidController;
    this-> encoder = Encoder(encoderPin1, encoderPin2);

    pinMode(pwmPin, OUTPUT);
    pinMode(directionPin1, OUTPUT);
    pinMode(directionPin2, OUTPUT);

    setMotorDutyCycle(0);
    brake();
}

float Motor::getRPM(long dt) {
    pulseCount = encoder.readAndReset();
    long delta = pulseCount - previousPulseCount;
    previousPulseCount = pulseCount;
    return (delta / (float)PULSE_PER_REVOLUTION) / dt * 6000.0;
};

void Motor::brake() {
    digitalWrite(directionPin1, LOW);
    digitalWrite(directionPin2, LOW);
}

void Motor::clockwise() {
    digitalWrite(directionPin1, HIGH);
    digitalWrite(directionPin2, LOW);
}

void Motor::anticlockwise() {
    digitalWrite(directionPin1, LOW);
    digitalWrite(directionPin2, HIGH);
}

void Motor::setMotorDutyCycle(int speed) {
    int motorSpeed = abs((speed / 100.0) * 255);
    if (speed > 0) anticlockwise(); else clockwise();
    analogWrite(pwmPin, motorSpeed);
}

void Motor::setMotorRPM(int rpm, long dt) {
    float currentRPM = getRPM(dt);
    if (rpm > 0) anticlockwise(); else clockwise();
    analogWrite(pwmPin, -pidController.adjustmentValue(rpm, currentRPM));
}
