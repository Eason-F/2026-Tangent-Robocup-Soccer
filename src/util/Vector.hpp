#pragma once

#include <Arduino.h>

class Vector {
    public:
        float x = 0;
        float y = 0;
        float angle = 0;
        float magnitude = 0;

        struct Position {};
        struct AngMag {};

        Vector();
        Vector(Position, const float &posX, const float &posY);
        Vector(AngMag, const float &angle, const float &length);

        Vector operator+(const Vector &vec);
        Vector operator-(const Vector &vec);
        Vector operator*(const float &n);
        Vector operator/(const float &n);

        Vector rotateBy(const float &angle);
};