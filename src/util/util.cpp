#include <util/util.hpp>
#include <util/Logger.hpp>

PIDController::PIDController(const float &kP, const float &kI, const float &kD, const float &min, const float &max) : 
    kP(kP), kI(kI), kD(kD), 
    max(max), min(min) {}

float PIDController::adjustmentValue(const float &dt, const float &error) {
    float derivative = (error - lastError) / dt;
    integral += error * dt;
    value = kP * error + kI * integral + kD * derivative;
    value = constrain(value, min, max);
    lastError = error;
    // Logger::queue("pid.value", value); Logger::queue("pid.error", error);
    // Logger::queue("KP", kP * error);
    // Logger::queue("KI", kI * integral);
    // Logger::queue("KD", kD * derivative);
    return value;
};

float PIDController::adjustmentValue(const float &dt, const float &target, const float &current) {
    return adjustmentValue(dt, target - current);
};

Vector::Vector() {
}

Vector::Vector(Position, const float &posX, const float &posY) {
    this-> x = posX;
    this-> y = posY;
    angle = atan2(posY, posX);
    magnitude = sqrt(pow(posX, 2) + pow(posY, 2));
}

Vector::Vector(AngMag, const float &angle, const float &length) {
    this-> angle = angle;
    this-> magnitude = length;
    x = length * cos(angle);
    y = length * sin(angle);
}

Vector Vector::operator+(const Vector &vec) {
    return Vector(Position {}, x + vec.x, y + vec.y);
}

Vector Vector::operator-(const Vector &vec) {
    return Vector(Position {}, x - vec.x, y - vec.y);
}

Vector Vector::operator*(const float &n) {
    return Vector(Position {}, x * n, y * n);
}

Vector Vector::operator/(const float &n) {
    return Vector(Position {}, x / n, y / n);
}

float wrapAngle180(const float angle) {
    float wrapped = std::fmod(angle + 180.0, 360.0);
    if (wrapped < 0.0) {
        wrapped += 360.0;
    }
    return wrapped - 180.0;
}

float mapRange(const float value, const float fromMin, const float fromMax, const float toMin, const float toMax) {
    if (fromMin == fromMax) return toMin; 
    
    return toMin + (value - fromMin) * (toMax - toMin) / (fromMax - fromMin);
}
