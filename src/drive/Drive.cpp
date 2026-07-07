#include <Arduino.h>
#include <drive/Drive.hpp>

Drive::Drive() :
    motor1(DIRECTION_PIN1_1, DIRECTION_PIN2_1, ENCODER_PIN1_1, ENCODER_PIN2_1, motorPID1),
    motor2(DIRECTION_PIN1_2, DIRECTION_PIN2_2, ENCODER_PIN1_2, ENCODER_PIN2_2, motorPID2),
    motor3(DIRECTION_PIN1_3, DIRECTION_PIN2_3, ENCODER_PIN1_3, ENCODER_PIN2_3, motorPID3),
    motor4(DIRECTION_PIN1_4, DIRECTION_PIN2_4, ENCODER_PIN1_4, ENCODER_PIN2_4, motorPID4) {}

void Drive::setup() {
    analogWriteResolution(8);
    motor1.setup();
    motor2.setup();
    motor3.setup();
    motor4.setup();
}

void Drive::moveInDirection(const float &dt, int directionDegrees, int rpm) {
    lastDirection = directionDegrees;
    motor1.setMotorRPM(cos(radians(directionDegrees + 315)) * rpm, dt);
    motor2.setMotorRPM(cos(radians(directionDegrees + 225)) * rpm, dt);
    motor3.setMotorRPM(cos(radians(directionDegrees + 45)) * rpm, dt);
    motor4.setMotorRPM(cos(radians(directionDegrees + 135)) * rpm, dt);
}

void Drive::moveToPoint(const float &dt, const int &rpm, const float &targetX, const float &targetY, OpticalOdometry &odometry) {
    float distance = sqrt(pow(targetX - odometry.getX(), 2) + pow(targetY - odometry.getY(), 2));
    float direction = degrees(atan2(targetX - odometry.getX(), targetY - odometry.getY()));
    int adjustment = positionPID.adjustmentValue(dt, 0, distance) * rpm;
    LOG("X", odometry.getX()); LOG("Y", odometry.getY()); LOG("dist", distance); LOG("dir", direction); LOG("target", adjustment);
    
    // motor1.setMotorRPM(adjustment, dt);
    moveInDirection(dt, direction, adjustment);
    LOG("RPM", motor1.angularVelocityRPM); LOG_PRINT("Done"); LOG_NEXT;
}

void Drive::moveToPoint(const float &dt, const int &rpm, const sfe_otos_pose2d_t &target, OpticalOdometry &odometry) {
    moveToPoint(dt, rpm, target.x, target.y, odometry);
}

void Drive::turnInDirection(const float &dt, int rpm) {
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

bool Drive::correctHeading(const float &dt, float heading) {
    if (headingCorrected(heading)) {
        return false;
    }

    int adjustmentRate = abs((heading - targetHeading)) * HEADING_ADJUSTMENT_MULTIPLIER + TURN_SPEED;
    adjustmentRate *= (heading > 0) ? -1 : 1;
    turnInDirection(dt, adjustmentRate);

    return true;
}
