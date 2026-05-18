#include <Arduino.h>

#include <drive/motor/Motor.hpp>

Motor::Motor() : directionPin1(-1), directionPin2(-1), encoderPin1(-1), encoderPin2(-1) {}

Motor::Motor(const int &directionPin1, const int &directionPin2) : 
    directionPin1(directionPin1), 
    directionPin2(directionPin2), 
    encoderPin1(-1), encoderPin2(-1) {}

Motor::Motor(const int &directionPin1, const int &directionPin2,  const int &encoderPin1, const int &encoderPin2, PIDController *pidController) :
    directionPin1(directionPin1),
    directionPin2(directionPin2),
    encoderPin1(encoderPin1),
    encoderPin2(encoderPin2),
    pidController(pidController) {}

void Motor::setup() {
    pinMode(directionPin1, OUTPUT);
    pinMode(directionPin2, OUTPUT);
    analogWriteFrequency(directionPin1, 20000);
    analogWriteFrequency(directionPin2, 20000);

    if ((encoderPin1 > 0 && encoderPin2 > 0) && (encoder == nullptr)) {
        encoder = new Encoder(encoderPin1, encoderPin2);
    }
    brake();
}

float Motor::getRPM(float dt) {
    long delta = encoder-> readAndReset();
    LOG_PRINT("count", delta);
    angularVelocityRPM = (delta / (float)PULSE_PER_REVOLUTION) * (60.0 / dt);
    return angularVelocityRPM;
};

void Motor::brake() {
    analogWrite(directionPin1, LOW);
    analogWrite(directionPin2, LOW);
}

void Motor::setMotorDutyCycle(int speed) {
    int motorSpeed = abs((speed / 100.0) * 255);
    if (speed > 0) {
        analogWrite(directionPin1, motorSpeed);
        analogWrite(directionPin2, 0);
    } else if (speed < 0) {
        analogWrite(directionPin1, 0);
        analogWrite(directionPin2, motorSpeed);
    } else {
        brake();
    }
}

void Motor::setMotorRPM(int rpm, float dt) {
    float currentRPM = getRPM(dt);
    if (rpm > 0) {
        analogWrite(directionPin1, pidController->adjustmentValue(dt, rpm, currentRPM));
        analogWrite(directionPin2, 0);
    } else if (rpm < 0) {
        analogWrite(directionPin1, 0);
        analogWrite(directionPin2, pidController->adjustmentValue(dt, rpm, currentRPM));
    } else {
        brake();
    }
}
