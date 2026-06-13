#pragma once
#include <Arduino.h>

class ColourSensor{
    public:
        ColourSensor(const int &pin);

        bool detectedEdge();

    private:
        static ColourSensor* instance;
        volatile bool onField = false;
        static void interruptWrapper();
        void onFallingEdge();

};

