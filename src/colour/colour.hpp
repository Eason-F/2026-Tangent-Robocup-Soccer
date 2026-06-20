#pragma once
#include <Arduino.h>

class ColourSensor{
    public:
        ColourSensor(const int &pin);
        void setup();
        void update(long elapsedMillis);
        bool detectedEdge();

    private:
        static constexpr uint8_t DEBOUNCE_BUFFER_MS = 50;
        const uint8_t pin;

        unsigned long accumulatedDetectionTime;
};

