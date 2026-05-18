#pragma once
#define ENCODER_USE_INTERRUPTS

#include <memory>
#include <util/util.hpp>
#include <Encoder.h>

class Motor {
    public:
        const static int PULSE_PER_REVOLUTION = 574;

        float angularVelocityRPM;

        Motor();
        Motor(const int &directionPin1, const int &directionPin2);
        Motor(const int &directionPin1, const int &directionPin2,  const int &encoderPin1, const int &encoderPin2, PIDController *pidController);
        void setup();

        float getRPM(float dt);

        void setMotorDutyCycle(int speed);
        void setMotorRPM(int rpm, float dt);

        void brake();

    private: 
        const int directionPin1;
        const int directionPin2;
        const int encoderPin1;
        const int encoderPin2;

        Encoder *encoder = nullptr;
        PIDController *pidController = nullptr;
};