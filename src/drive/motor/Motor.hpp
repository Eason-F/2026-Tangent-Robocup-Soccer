#pragma once

#include <util/util.hpp>

class Motor {
    public:
        const static int PULSE_PER_REVOLUTION = 18;

        int angularVelocityRPM;
        volatile int pulseCount;

        Motor();

        Motor(const int &pwmPin, const int &directionPin, const int &pulsePin);

        void incrementPulseCount();

        void setMotorSpeed(int speed);

    private: 
        int pwmPin;
        int directionPin;
        int pulsePin;
};