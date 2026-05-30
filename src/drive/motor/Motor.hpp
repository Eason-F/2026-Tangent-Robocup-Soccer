#pragma once

#include <util/util.hpp>
#include <Encoder.h>

class Motor {
    public:
        static constexpr int PULSE_PER_REVOLUTION = 1600;

        float angularVelocityRPM;

        ~Motor();
        Motor(const int &directionPin1, const int &directionPin2);
        Motor(const int &directionPin1, const int &directionPin2,  const int &encoderPin1, const int &encoderPin2, PIDController *pidController);
        void setup();

        float getRPM(float dt);

        void setMotorDutyCycle(int speed);
        void setMotorRPM(int rpm, float dt);

        void brake();

    private: 
        const int DIRECTION_PIN1;
        const int DIRECTION_PIN2;
        const int ENCODER_PIN1;
        const int ENCODER_PIN2;

        Encoder *encoder = nullptr;
        PIDController *pidController = nullptr;
};