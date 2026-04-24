#include <util/util.hpp>

PIDController::PIDController(const float &kP, const float &kI, const float &kD) {
    this-> kP = kP;
    this-> kI = kI;
    this-> kD = kD;
};

float PIDController::adjustmentValue(float current, float target) {
    float error = target - current;
    return error * kP;
};

Vector::Vector(Position, const float &posX, const float &posY) {
    this-> x = posX;
    this-> y = posY;
    angle = acos(posY / posX);
    length = sqrt(pow(posX, 2) + pow(posY, 2));
}

Vector::Vector(AngMag, const float &angle, const float &length) {
    this-> angle = angle;
    this-> length = length;
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