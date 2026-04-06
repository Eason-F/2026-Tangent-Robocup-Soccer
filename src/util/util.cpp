#include <util/util.hpp>

PIDController::PIDValues::PIDValues(const float &kP, const float &kI, const float &kD) {
    this-> kP = kP;
    this-> kI = kI;
    this-> kD = kD;
}

PIDController::PIDController() {};

PIDController::PIDController(const float &kP, const float &kI, const float &kD) {
    pidValues.kP = kP;
    pidValues.kI = kI;
    pidValues.kD = kD;
};

PIDController::PIDController(PIDValues pidValues) {
    this-> pidValues = pidValues;
};

float PIDController::adjustmentValue(float current, float target) {
    float error = target - current;
    return error * pidValues.kP;
};