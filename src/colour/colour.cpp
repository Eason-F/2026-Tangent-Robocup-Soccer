#include <colour/colour.hpp>

ColourSensor::ColourSensor(const int &pinFront, const int &pinRight, const int &pinBack, const int &pinLeft) :
    front(pinFront, 0.0f),
    right(pinRight, PI / 2.0f),
    back(pinBack, PI),
    left(pinLeft, 3.0f * PI / 2.0f) {}

void ColourSensor::setup() {
    front.setup();
    right.setup();
    back.setup();
    left.setup();
}

void ColourSensor::update(long elapsedMillis) {
    front.update(elapsedMillis);
    right.update(elapsedMillis);
    back.update(elapsedMillis);
    left.update(elapsedMillis);
}

bool ColourSensor::detectedEdge() {
    return front.detectedEdge()
        || right.detectedEdge()
        || back.detectedEdge()
        || left.detectedEdge();
}

Vector ColourSensor::getVector() {
    return front.getVector()
        + right.getVector()
        + back.getVector()
        + left.getVector();
}
