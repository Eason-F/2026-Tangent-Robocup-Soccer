#include <Arduino.h>
#include <drive/Drive.hpp>

Drive::Drive() {
    motor1 = Motor(PWM_PIN_1, DIRECTION_PIN_1, PULSE_PIN_1);
    motor2 = Motor(PWM_PIN_2, DIRECTION_PIN_2, PULSE_PIN_2);
    motor3 = Motor(PWM_PIN_3, DIRECTION_PIN_3, PULSE_PIN_3);
    motor4 = Motor(PWM_PIN_4, DIRECTION_PIN_4, PULSE_PIN_4);
}

void Drive::moveInDirection(int directionDegrees, int speed) {
    motor1.setMotorSpeed(cos(radians(directionDegrees + 315)) * speed);
    motor2.setMotorSpeed(cos(radians(directionDegrees + 45)) * speed);
    motor3.setMotorSpeed(cos(radians(directionDegrees + 135)) * speed);
    motor4.setMotorSpeed(cos(radians(directionDegrees + 225)) * speed);
}