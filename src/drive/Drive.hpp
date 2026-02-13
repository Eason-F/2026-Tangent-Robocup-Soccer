#pragma once

#include <drive/motor/Motor.hpp>
#include <util/util.hpp>

class Drive {
    public:
        Motor motor1;
        Motor motor2;
        Motor motor3;
        Motor motor4;

        Drive();

        void moveInDirection(int directionDegrees, int speed);

        void turnInDirection(int direction, int speed);

    private: 
        const int PWM_PIN_1 = 11;
        const int DIRECTION_PIN_1 = 12;
        const int PULSE_PIN_1 = 13;

        const int PWM_PIN_2 = 9;
        const int DIRECTION_PIN_2 = 8;
        const int PULSE_PIN_2 = 10;

        const int PWM_PIN_3 = 6;
        const int DIRECTION_PIN_3 = 7;
        const int PULSE_PIN_3 = 5;

        const int PWM_PIN_4 = 3;
        const int DIRECTION_PIN_4 = 4;
        const int PULSE_PIN_4 = 2;
};