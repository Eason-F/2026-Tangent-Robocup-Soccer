#pragma once
#include <Arduino.h>

class ColourSensor{
    public:
        ColourSensor(const int &pin);

        bool detectedEdge();

    private:
        static ColourSensor* ColourSensor::instance;
        volatile bool onField;
        static void ColourSensor::interruptWrapper();
        void onFallingEdge();

};


