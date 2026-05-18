#pragma once

#include <Arduino.h>

#define LOG_NEXT Serial.println();
#define LOG_PRINT(text) Serial.print(text);
#define LOG(header, text) LOG_PRINT(header); LOG_PRINT(": "); LOG_PRINT(text); LOG_PRINT("\t|\t");

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