#include <colour/colour.hpp>


ColourSensor* ColourSensor::instance = nullptr;

void ColourSensor::onFallingEdge() {
    onField = true;
}

ColourSensor::ColourSensor(const int &pin) {
    instance = this;
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(pin, ColourSensor::interruptWrapper, RISING);
}

void ColourSensor::interruptWrapper() {
    if (instance != nullptr) {
        instance->onFallingEdge();
    }
}

bool ColourSensor::detectedEdge() {
    noInterrupts();
    bool detected = onField;
    onField = false;
    interrupts();
    return detected;
}
