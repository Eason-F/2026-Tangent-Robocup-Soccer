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
        const int DIRECTION_PIN1_1 = 2;
        const int DIRECTION_PIN2_1 = 3;
        const int ENCODER_PIN1_1 = 33;
        const int ENCODER_PIN2_1 = 34;

        const int DIRECTION_PIN1_2 = 4;
        const int DIRECTION_PIN2_2 = 5;
        const int ENCODER_PIN1_2 = 35;
        const int ENCODER_PIN2_2 = 36;

        const int DIRECTION_PIN1_3 = 6;
        const int DIRECTION_PIN2_3 = 7;
        const int ENCODER_PIN1_3 = 37;
        const int ENCODER_PIN2_3 = 37;

        const int DIRECTION_PIN1_4 = 8;
        const int DIRECTION_PIN2_4 = 9;
        const int ENCODER_PIN1_4 = 39;
        const int ENCODER_PIN2_4 = 40;

};