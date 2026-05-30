#include <Arduino.h>

#include <drive/motor/Motor.hpp>

Motor::~Motor() {
    delete encoder;
}

Motor::Motor(const int &directionPin1, const int &directionPin2) : 
    DIRECTION_PIN1(directionPin1), 
    DIRECTION_PIN2(directionPin2), 
    ENCODER_PIN1(-1), ENCODER_PIN2(-1), 
    encoder(nullptr), pidController(nullptr) {}

Motor::Motor(const int &directionPin1, const int &directionPin2,  const int &encoderPin1, const int &encoderPin2, PIDController *pidController) :
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

    LOG_PRINT(ENCODER_PIN1); LOG_PRINT(ENCODER_PIN2);
    if ((ENCODER_PIN1 > 0 && ENCODER_PIN2 > 0) && (encoder == nullptr)) {
        encoder = new Encoder(ENCODER_PIN1, ENCODER_PIN2);
    }
    Serial.println((uint32_t)encoder, HEX);
    brake();
}

float Motor::getRPM(float dt) {
    if (encoder == nullptr || dt <= 0.0f)
        return 0.0f;

    long delta = encoder-> readAndReset();
    LOG("count", delta);
    angularVelocityRPM = (delta / (float)PULSE_PER_REVOLUTION) * (60.0 / dt);
    return angularVelocityRPM;
};

void Motor::brake() {
    analogWrite(DIRECTION_PIN1, 0);
    analogWrite(DIRECTION_PIN2, 0);
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

void Motor::setMotorRPM(int rpm, float dt) {
    if (pidController == nullptr)
        return;

    float currentRPM = abs(getRPM(dt));
    if (rpm < 0) {
        analogWrite(DIRECTION_PIN1, (int) pidController->adjustmentValue(dt, currentRPM, abs(rpm)));
        analogWrite(DIRECTION_PIN2, 0);
    } else if (rpm > 0) {
        analogWrite(DIRECTION_PIN1, 0);
        analogWrite(DIRECTION_PIN2, (int) pidController->adjustmentValue(dt, currentRPM, abs(rpm)));
    } else {
        brake();
    }
}
