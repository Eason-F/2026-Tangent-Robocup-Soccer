#pragma once

#include <Arduino.h>

#define LOG_NEXT Serial.println();
#define LOG_PRINT(text) Serial.print(text);
#define LOG(header, text) LOG_PRINT(header); LOG_PRINT(": "); LOG_PRINT(text); LOG_PRINT("  | ");

class PIDController {
    private:
        const float kP = 0;
        const float kI = 0;
        const float kD = 0;
        const float max = 0.0f;
        const float min = 0.0f;

        float value;
        float lastError;
        float integral;

    public:
        PIDController(const float &kP, const float &kI, const float &kD, const float &min, const float &max);

        float adjustmentValue(float dt, float target, float current);
};

class Vector {
    public:
        float x = 0;
        float y = 0;
        float angle = 0;
        float length = 0;

        struct Position{}; // structs for constructor differentiation
        struct AngMag{};
        Vector();
        Vector(Position, const float &posX, const float &posY);
        Vector(AngMag, const float &angle, const float &length);

        Vector operator+(const Vector &vec);
        Vector operator-(const Vector &vec);
        Vector operator*(const float &n);
        Vector operator/(const float &n);
};