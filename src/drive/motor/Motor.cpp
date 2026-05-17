#include <Arduino.h>

#include <drive/motor/Motor.hpp>

Motor::Motor() {
    this-> directionPin1 = 99;
    this-> directionPin2 = 99;
}

Motor::Motor(const int &directionPin1, const int &directionPin2) {
    this-> directionPin1 = directionPin1;
    this-> directionPin2 = directionPin2;

    pinMode(directionPin1, OUTPUT);
    pinMode(directionPin2, OUTPUT);

    setMotorDutyCycle(0);
    brake();
};

Motor::Motor(const int &directionPin1, const int &directionPin2,  const int &encoderPin1, const int &encoderPin2, PIDController &pidController) {
    this-> directionPin1 = directionPin1;
    this-> directionPin2 = directionPin2;
    this-> encoderPin1 = encoderPin1;
    this-> encoderPin2 = encoderPin2;
    this-> pidController = pidController;
    this-> encoder = Encoder(encoderPin1, encoderPin2);

    pinMode(directionPin1, OUTPUT);
    pinMode(directionPin2, OUTPUT);
    analogWriteFrequency(directionPin1, 20000);
    analogWriteFrequency(directionPin2, 20000);

    setMotorDutyCycle(0);
}

float Motor::getRPM(float dt) {
    long delta = encoder.readAndReset();
    LOG_PRINT("count", delta);
    angularVelocityRPM = (delta / (float)PULSE_PER_REVOLUTION) * (60.0 / dt);
    return angularVelocityRPM;
};

void Motor::brake() {
    digitalWrite(directionPin1, LOW);
    digitalWrite(directionPin2, LOW);
}

void Motor::setMotorDutyCycle(int speed) {
    int motorSpeed = abs((speed / 100.0) * 255);
    if (speed > 0) {
        analogWrite(directionPin1, motorSpeed);
        analogWrite(directionPin2, 0);
    } else {
        analogWrite(directionPin1, 0);
        analogWrite(directionPin2, motorSpeed);
    }
}

void Motor::setMotorRPM(int rpm, long dt) {
    float currentRPM = getRPM(dt);
    if (rpm > 0) {
        analogWrite(directionPin1, pidController.adjustmentValue(rpm, currentRPM));
        analogWrite(directionPin2, 0);
    } else {
        analogWrite(directionPin1, 0);
        analogWrite(directionPin2, pidController.adjustmentValue(rpm, currentRPM));
    }
    
}
