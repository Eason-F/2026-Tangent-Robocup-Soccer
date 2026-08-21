#pragma once

#include <Arduino.h>
#include <SparkFun_Qwiic_OTOS_Arduino_Library.h>

#define LOG_NEXT Serial.println();
#define LOG_PRINT(text) Serial.print(text);
#define LOG(header, text) LOG_PRINT(header) LOG_PRINT(": ") LOG_PRINT(text) LOG_PRINT("  | ")

#define conditionallyBreakLoop(bool) if (bool) {return;}

struct Position2D {
    float x = 0.0f;
    float y = 0.0f;

    constexpr Position2D() = default;
    constexpr Position2D(float x, float y) : x(x), y(y) {}

    constexpr sfe_otos_pose2d_t toPose2D(float heading) const {
        return {x, y, heading};
    }
};

namespace util {
    inline float wrapAngle180(const float angle) {
        return std::remainder(angle, 360.0);
    }

    inline float mapRange(const float value, const float fromMin, const float fromMax, const float toMin, const float toMax) {
        if (fromMin == fromMax) return toMin; 
        
        return toMin + (value - fromMin) * (toMax - toMin) / (fromMax - fromMin);
    }
}

