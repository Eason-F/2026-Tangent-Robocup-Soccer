#include <Arduino.h>
#include <drive/Drive.hpp>

Drive::Drive() {
    motor1 = Motor(PWM_PIN_1, DIRECTION_PIN1_1, DIRECTION_PIN2_1, ENCODER_PIN1_1, ENCODER_PIN2_1, pidController);
    motor2 = Motor(PWM_PIN_2, DIRECTION_PIN1_2, DIRECTION_PIN2_2, ENCODER_PIN1_2, ENCODER_PIN2_2, pidController);
    motor3 = Motor(PWM_PIN_3, DIRECTION_PIN1_3, DIRECTION_PIN2_3, ENCODER_PIN1_3, ENCODER_PIN2_3, pidController);
    motor4 = Motor(PWM_PIN_4, DIRECTION_PIN1_4, DIRECTION_PIN2_4, ENCODER_PIN1_4, ENCODER_PIN2_4, pidController);

    pinMode(STBY_PIN_1, OUTPUT);
    pinMode(STBY_PIN_2, OUTPUT);
    digitalWrite(STBY_PIN_1, HIGH);
    digitalWrite(STBY_PIN_2, HIGH);
}

void Drive::moveInDirection(int directionDegrees, int speed) {
    motor1.setMotorDutyCycle(cos(radians(directionDegrees + 315)) * speed);
    motor2.setMotorDutyCycle(cos(radians(directionDegrees + 45)) * speed);
    motor3.setMotorDutyCycle(cos(radians(directionDegrees + 135)) * speed);
    motor4.setMotorDutyCycle(cos(radians(directionDegrees + 225)) * speed);
}