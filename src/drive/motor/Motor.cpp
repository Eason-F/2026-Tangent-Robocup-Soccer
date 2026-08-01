#include <Arduino.h>

#include <drive/motor/Motor.hpp>

Motor::~Motor() {
    delete encoder;
}

Motor::Motor(const int &directionPin1, const int &directionPin2,  const int &encoderPin1, const int &encoderPin2, PIDController &pidController) :
    DIRECTION_PIN1(directionPin1),
    DIRECTION_PIN2(directionPin2),
    ENCODER_PIN1(encoderPin1),
    ENCODER_PIN2(encoderPin2),
    encoder(nullptr),
    pidController(pidController) {}

void Motor::setup() {
    pinMode(DIRECTION_PIN1, OUTPUT);
    pinMode(DIRECTION_PIN2, OUTPUT);
    analogWriteFrequency(DIRECTION_PIN1, 20000);
    analogWriteFrequency(DIRECTION_PIN2, 20000);

    if ((ENCODER_PIN1 > 0 && ENCODER_PIN2 > 0) && (encoder == nullptr)) {
        encoder = new Encoder(ENCODER_PIN1, ENCODER_PIN2);
    }
    brake();
}

float Motor::getRPM(const float &dt) {
    if (encoder == nullptr || dt <= 0.0f)
        return 0.0f;

    long delta = encoder-> readAndReset();
    angularVelocityRPM = (delta / (float)PULSE_PER_REVOLUTION) * (60.0 / dt);
    return angularVelocityRPM;
};

void Motor::brake() {
    analogWrite(DIRECTION_PIN1, 255);
    analogWrite(DIRECTION_PIN2, 255);
    lastInput = 0;
}

void Motor::setMotorDutyCycle(int speed) {
    int motorSpeed = abs((speed / 100.0) * 255);
    if (speed > 0) {
        analogWrite(DIRECTION_PIN1, motorSpeed);
        analogWrite(DIRECTION_PIN2, 0);
    } else if (speed < 0) {
        analogWrite(DIRECTION_PIN1, 0);
        analogWrite(DIRECTION_PIN2, motorSpeed);
    } else {
        brake();
    }
}

void Motor::setMotorRPM(int rpm, const float &dt) {
    float currentRPM = abs(getRPM(dt));
    lastInput += (int) pidController.adjustmentValue(dt, abs(rpm), currentRPM);
    lastInput = min(lastInput, 255);
    if (rpm < 0) {
        analogWrite(DIRECTION_PIN1, lastInput);
        analogWrite(DIRECTION_PIN2, 0);
    } else if (rpm > 0) {
        analogWrite(DIRECTION_PIN1, 0);
        analogWrite(DIRECTION_PIN2, lastInput);
    } else {
        brake();
    }
}
