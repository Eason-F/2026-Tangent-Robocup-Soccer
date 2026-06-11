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

float Drive::motorSpeedAtAngle(const float &movementDirection, const float &offsetAngle) {
    return cos(radians(movementDirection + offsetAngle));
}

void Drive::moveInDirection(const float &dt, const int &directionDegrees, const int &rpm, const float &heading) {
    int headingAdjustment = heading * HEADING_MULT;
    motor1.setMotorRPM(motorSpeedAtAngle(directionDegrees, -45) * rpm + headingAdjustment, dt);
    motor2.setMotorRPM(motorSpeedAtAngle(directionDegrees, 45) * rpm + headingAdjustment, dt);
    motor3.setMotorRPM(motorSpeedAtAngle(directionDegrees, -135) * rpm + headingAdjustment, dt);
    motor4.setMotorRPM(motorSpeedAtAngle(directionDegrees, 135) * rpm + headingAdjustment, dt);
}

void Drive::turnInDirection(const float &dt, const int &rpm) {
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