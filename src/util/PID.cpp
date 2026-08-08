#include <Arduino.h>

#include <util/PID.hpp>

PIDController::PIDController(const float &kP, const float &kI, const float &kD, const float &min, const float &max) :
    kP(kP), kI(kI), kD(kD),
    max(max), min(min) {}

float PIDController::adjustmentValue(const float &dt, const float &target, const float &current) {
    float error = target - current;
    float derivative = (error - lastError) / dt;
    integral += error * dt;
    value = kP * error + kI * integral + kD * derivative;
    value = constrain(value, min, max);
    lastError = error;
    return value;
}
