#include <Arduino.h>
#include <drive/Drive.hpp>

Drive::Drive() {
    motor1 = Motor(PWM_PIN_1, DIRECTION_PIN1_1, DIRECTION_PIN2_1);
    motor2 = Motor(PWM_PIN_2, DIRECTION_PIN1_2, DIRECTION_PIN2_2);
    motor3 = Motor(PWM_PIN_3, DIRECTION_PIN1_3, DIRECTION_PIN2_3);
    motor4 = Motor(PWM_PIN_4, DIRECTION_PIN1_4, DIRECTION_PIN2_4);

    pinMode(STBY_PIN, OUTPUT);
    digitalWrite(STBY_PIN, HIGH);
}

void Drive::moveInDirection(int directionDegrees, int speed) {
    motor1.setMotorDutyCycle(cos(radians(directionDegrees + 315)) * speed);
    motor2.setMotorDutyCycle(cos(radians(directionDegrees + 225)) * speed);
    motor3.setMotorDutyCycle(cos(radians(directionDegrees + 45)) * speed);
    motor4.setMotorDutyCycle(cos(radians(directionDegrees + 135)) * speed);
}