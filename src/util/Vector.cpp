#include <Arduino.h>

#include <util/Vector.hpp>

Vector::Vector() {
}

Vector::Vector(Position, const float &posX, const float &posY) {
    x = posX;
    y = posY;
    angle = atan2(posY, posX);
    magnitude = sqrt(pow(posX, 2) + pow(posY, 2));
}

Vector::Vector(AngMag, const float &angle, const float &length) {
    this->angle = angle;
    magnitude = length;
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
