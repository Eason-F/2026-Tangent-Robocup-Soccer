#pragma once

#include <drive/motor/Motor.hpp>
#include <util/util.hpp>

class Drive {
    public:
        Motor motor1;
        Motor motor2;
        Motor motor3;
        Motor motor4;

        PIDController pidController = PIDController(1, 0, 0);

        Drive();

        void moveInDirection(int directionDegrees, int speed);

        void turnInDirection(int direction, int speed);

    private: 
        const int STBY_PIN = 8;

        const int PWM_PIN_1 = 4;
        const int DIRECTION_PIN1_1 = 2;
        const int DIRECTION_PIN2_1 = 3;
        const int ENCODER_PIN1_1 = 25;
        const int ENCODER_PIN2_1 = 26;

        const int PWM_PIN_2 = 7;
        const int DIRECTION_PIN1_2 = 5;
        const int DIRECTION_PIN2_2 = 6;
        const int ENCODER_PIN1_2 = 27;
        const int ENCODER_PIN2_2 = 28;

        const int PWM_PIN_3 = 11;
        const int DIRECTION_PIN1_3 = 9;
        const int DIRECTION_PIN2_3 = 10;
        const int ENCODER_PIN1_3 = 29;
        const int ENCODER_PIN2_3 = 30;

        const int PWM_PIN_4 = 23;
        const int DIRECTION_PIN1_4 = 21;
        const int DIRECTION_PIN2_4 = 22;
        const int ENCODER_PIN1_4 = 31;
        const int ENCODER_PIN2_4 = 32;

};