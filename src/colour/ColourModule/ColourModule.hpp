#pragma once

#include <Arduino.h>
#include <util/Vector.hpp>

class ColourModule {
    public:
        ColourModule(const int &pin, const float &direction);
        void setup();
        void update(long elapsedMillis);
        bool detectedEdge();
        Vector getVector();

    private:
        const int pin;
        const float direction;
        
        static constexpr uint8_t DEBOUNCE_BUFFER_MS = 5;
        unsigned long detectionBufferRemaining = 0;
};

