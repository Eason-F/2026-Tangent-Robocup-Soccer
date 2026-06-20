#include <Arduino.h>
#include <drive/Drive.hpp>

Drive::Drive() :
    motor1(DIRECTION_PIN1_1, DIRECTION_PIN2_1, ENCODER_PIN1_1, ENCODER_PIN2_1, &pidController1),
    motor2(DIRECTION_PIN1_2, DIRECTION_PIN2_2, ENCODER_PIN1_2, ENCODER_PIN2_2, &pidController2),
    motor3(DIRECTION_PIN1_3, DIRECTION_PIN2_3, ENCODER_PIN1_3, ENCODER_PIN2_3, &pidController3),
    motor4(DIRECTION_PIN1_4, DIRECTION_PIN2_4, ENCODER_PIN1_4, ENCODER_PIN2_4, &pidController4) {}

void Drive::setup() {
    analogWriteResolution(8);
    motor1.setup();
    motor2.setup();
    motor3.setup();
    motor4.setup();
}

void Drive::moveInDirection(float dt, int directionDegrees, int rpm) {
    lastDirection = directionDegrees;
    motor1.setMotorRPM(cos(radians(directionDegrees + 315)) * rpm, dt);
    motor2.setMotorRPM(cos(radians(directionDegrees + 225)) * rpm, dt);
    motor3.setMotorRPM(cos(radians(directionDegrees + 45)) * rpm, dt);
    motor4.setMotorRPM(cos(radians(directionDegrees + 135)) * rpm, dt);
}

void Drive::turnInDirection(float dt, int rpm) {
    motor1.setMotorRPM(rpm, dt);
    motor2.setMotorRPM(rpm, dt);
    motor3.setMotorRPM(rpm, dt);
    motor4.setMotorRPM(rpm, dt);
}

void Drive::stop() {
    motor1.brake();
    motor2.brake();
    motor3.brake();
    motor4.brake();
}

bool Drive::headingCorrected(float heading) {
    return abs(heading - targetHeading) <= HEADING_TOLERANCE_DEGREES;
}

void Drive::correctHeading(float dt, float heading) {
    if (headingCorrected(heading)) {
        return;
    }

    int adjustmentRate = heading * HEADING_ADJUSTMENT_MULTIPLIER + TURN_SPEED;
    adjustmentRate *= (heading > 0) ? -1 : 1;
    turnInDirection(dt, adjustmentRate);

    LOG("Adjusting rate:", adjustmentRate); LOG_NEXT;
}
