#pragma once

#include <Arduino.h>
#include <util/Vector.hpp>

#include "colourModule/ColourModule.hpp"

class ColourSensor {
    public:
        ColourSensor(const int &pinFront = 22, const int &pinRight = 23, const int &pinBack = 26, const int &pinLeft = 27);
        void setup();
        void update(long elapsedMillis);

        bool detectedEdge();
        Vector getVector();
        float getDirectionDegrees();

        ColourModule front;
        ColourModule right;
        ColourModule back;
        ColourModule left;
};
