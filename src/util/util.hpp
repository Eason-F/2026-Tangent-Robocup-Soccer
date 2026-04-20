#pragma once

#include <Arduino.h>

class PIDController {
    private:
        float kP;
        float kI;
        float kD;

    public:
        PIDController();
        PIDController(const float &kP, const float &kI, const float &kD);

        float adjustmentValue(float target, float error);
};

class Vector {
    public:
        float x;
        float y;
        float angle;
        float length;

        struct Position{}; // structs for constructor differentiation
        struct AngDir{};
        Vector(Position, const float &posX, const float &posY);
        Vector(AngDir, const float &angle, const float &length);

        Vector operator+(const Vector &vec);
        Vector operator-(const Vector &vec);
        Vector operator*(const float &n);
        Vector operator/(const float &n);
};