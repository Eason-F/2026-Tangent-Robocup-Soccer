#pragma once

#include <drive/motor/Motor.hpp>
#include <util/util.hpp>

#define ENCODER_USE_INTERRUPTS

class Drive {
    public:
        static constexpr float pidKP = 0.2;
        static constexpr float pidKI = 0;
        static constexpr float pidKD = 0;
        static constexpr float pidMax = 255;
        static constexpr float pidMin = 0;

        PIDController pidController1 = PIDController(pidKP, pidKI, pidKD, pidMin, pidMax);
        PIDController pidController2 = PIDController(pidKP, pidKI, pidKD, pidMin, pidMax);
        PIDController pidController3 = PIDController(pidKP, pidKI, pidKD, pidMin, pidMax);
        PIDController pidController4 = PIDController(pidKP, pidKI, pidKD, pidMin, pidMax);

        Motor motor1;
        Motor motor2;
        Motor motor3;
        Motor motor4;

        Drive();
        void setup();

        void update(float dt);
        void moveInDirection(float dt, int directionDegrees, int rpm);
        void turnInDirection(float dt, int rpm);
        void stop();

    private: 
        static constexpr int DIRECTION_PIN1_1 = 2;
        static constexpr int DIRECTION_PIN2_1 = 3;
        static constexpr int ENCODER_PIN1_1 = 33;
        static constexpr int ENCODER_PIN2_1 = 34;

        static constexpr int DIRECTION_PIN1_2 = 4;
        static constexpr int DIRECTION_PIN2_2 = 5;
        static constexpr int ENCODER_PIN1_2 = 35;
        static constexpr int ENCODER_PIN2_2 = 36;

        static constexpr int DIRECTION_PIN1_3 = 6;
        static constexpr int DIRECTION_PIN2_3 = 7;
        static constexpr int ENCODER_PIN1_3 = 37;
        static constexpr int ENCODER_PIN2_3 = 38;

        static constexpr int DIRECTION_PIN1_4 = 8;
        static constexpr int DIRECTION_PIN2_4 = 9;
        static constexpr int ENCODER_PIN1_4 = 39;
        static constexpr int ENCODER_PIN2_4 = 40;

};
