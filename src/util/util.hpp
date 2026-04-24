#pragma once

#include <Arduino.h>

#define LOG_PRINT(header, text) Serial.print(header); Serial.print(": "); Serial.print(text); Serial.print("\t|\t")
#define LOG_NEXT Serial.println();

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

        float adjustmentValue(float target, float error);
};