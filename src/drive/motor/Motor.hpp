#pragma once
#define ENCODER_USE_INTERRUPTS

#include <util/util.hpp>
#include <Encoder.h>

class Motor {
    public:
        const static int PULSE_PER_REVOLUTION = 574;

        float angularVelocityRPM;

        Motor();
        Motor(const int &directionPin1, const int &directionPin2);
        Motor(const int &directionPin1, const int &directionPin2,  const int &encoderPin1, const int &encoderPin2, PIDController &pidController);

        float getRPM(float dt);

        void setMotorDutyCycle(int speed);
        void setMotorRPM(int rpm, long dt);

        void brake();

    private: 
        int directionPin1;
        int directionPin2;
        int encoderPin1;
        int encoderPin2;

        Encoder encoder = Encoder(33, 34);
        PIDController pidController = PIDController(0, 0, 0);
};