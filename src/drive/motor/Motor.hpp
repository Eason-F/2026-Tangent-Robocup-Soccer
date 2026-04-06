#pragma once

#include <util/util.hpp>
#include <Encoder.h>

class Motor {
    public:
        const static int PULSE_PER_REVOLUTION = 442;

        int angularVelocityRPM;
        long pulseCount;
        long previousPulseCount = 0;

        Motor();
        Motor(const int &pwmPin, const int &directionPin1, const int &directionPin2);
        Motor(const int &pwmPin, const int &directionPin1, const int &directionPin2,  const int &encoderPin1, const int &encoderPin2, PIDController pidController);

        float getRPM(long dt);

        void setMotorDutyCycle(int speed);
        void setMotorRPM(int rpm, long dt);

        void brake();
        void clockwise();
        void anticlockwise();

    private: 
        int pwmPin;
        int directionPin1;
        int directionPin2;
        int encoderPin1;
        int encoderPin2;

        Encoder encoder = Encoder(99, 99);
        PIDController pidController = PIDController(0, 0, 0);
};