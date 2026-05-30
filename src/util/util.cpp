#include <util/util.hpp>

PIDController::PIDController(const float &kP, const float &kI, const float &kD, const float &min, const float &max) : 
    kP(kP), kI(kI), kD(kD), 
    max(max), min(min) {}

float PIDController::adjustmentValue(float dt, float current, float target) {
    float error = target - current;
    float derivative = (error - lastError) / dt;
    value += kP * error + kI * integral + kD * derivative;
    value = constrain(value, min, max);
    lastError = error;
    LOG("pid value", value);
    return value;
};