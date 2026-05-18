#pragma once

#include <Arduino.h>

#define LOG_NEXT Serial.println();
#define LOG_PRINT(text) Serial.print(text);
#define LOG(header, text) LOG_PRINT(header); LOG_PRINT(": "); LOG_PRINT(text); LOG_PRINT("\t|\t");

class Direction {
    public:
        const static int CLOCKWISE = 1;
        const static int ANTICLOCKWISE = -1;

};

class PIDController {
    private:
        struct PIDValues {
            float kP;
            float kI;
            float kD;

            PIDValues(const float &kP, const float &kI, const float &kD);
        };
    public:
        PIDValues pidValues = PIDValues(0, 0, 0);
        
        PIDController();
        PIDController(const float &kP, const float &kI, const float &kD);
        PIDController(PIDValues pidValues);

        float adjustmentValue(float target, float current);
};