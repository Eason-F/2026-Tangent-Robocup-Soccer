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
    // LOG("pid value", value);
    return value;
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
