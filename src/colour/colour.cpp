#include <colour/colour.hpp>

ColourSensor::ColourSensor(const int &pin) : pin(pin) {}

void ColourSensor::setup() {
    pinMode(pin, INPUT_PULLUP);
}

void ColourSensor::update(long elapsedMillis) {
    if (!digitalReadFast(22)) {
        accumulatedDetectionTime = 0; return;
    }
    accumulatedDetectionTime += elapsedMillis;
}

bool ColourSensor::detectedEdge() {
    return accumulatedDetectionTime >= DEBOUNCE_BUFFER_MS;
}  
