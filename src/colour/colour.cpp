#include <colour/colour.hpp>


ColourSensor* ColourSensor::instance = nullptr;

// void ColourSensor::onFallingEdge() {
//     onField = true;
// }

ColourSensor::ColourSensor(const int &pin) {
    instance = this;
    pinMode(pin, INPUT_PULLUP);
    // attachInterrupt(pin, ColourSensor::interruptWrapper, RISING);
}

// void ColourSensor::interruptWrapper() {
//     if (instance != nullptr) {
//         instance->onFallingEdge();
//     }
// }

bool ColourSensor::detectedEdge() {
    bool currentState = digitalReadFast(22);
    if (currentState) {
        delay(50);
        currentState = digitalReadFast(22);
        if (currentState) {
            return true;
}
    return false;
    }
}

bool ColourSensor::sensorState() {
    return digitalRead(22);
}
