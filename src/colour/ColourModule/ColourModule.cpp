#include "ColourModule.hpp"

ColourModule::ColourModule(const int &pin, const float &direction): pin(pin), direction(direction) {}

void ColourModule::setup() {
    pinMode(pin, INPUT_PULLUP);
}

void ColourModule::update(long elapsedMillis) {
    if (digitalReadFast(pin)) {
        detectionBufferRemaining = DEBOUNCE_BUFFER_MS;
        return;
    }

    const unsigned long elapsed = elapsedMillis > 0 ? static_cast<unsigned long>(elapsedMillis) : 0;

    if (elapsed >= detectionBufferRemaining) {
        detectionBufferRemaining = 0;
    } else {
        detectionBufferRemaining -= elapsed;
    }
}

bool ColourModule::detectedEdge() {
    return detectionBufferRemaining > 0;
} 

Vector ColourModule::getVector() {
    return Vector(Vector::AngMag{}, direction, detectedEdge() ? 1.0f : 0.0f);
}
