#include <qikeasy/QikEasy.hpp>

QikEasy::QikEasy(TwoWire &wirePort) {
    wirePort.begin();
    this-> wirePort = &wirePort;
}

uint16_t QikEasy::signalStrength(const int &receiverID) {
    const uint8_t *moduleAddress = &MODULE_ADDRESSES[static_cast<int>(floor((receiverID + 1) / 5))];

    wirePort-> beginTransmission(*moduleAddress);
    wirePort-> write(RECEIVER_REGISTERS[(receiverID + 1) % 5 - 1]);
    wirePort-> endTransmission();
    
    uint16_t strength = 0;
    wirePort-> requestFrom(*moduleAddress, static_cast<uint8_t>(2));
    if (wirePort-> available() >= 2) {
        strength = wirePort-> read() << 8;
        strength |= wirePort-> read();
    }
    return strength;
}

void QikEasy::updateReadings() {
    for (int i = 0; i < 10; i++) {
        uint16_t strength = signalStrength(i);
        float direction = (i * PI / 5) - (2 * PI) / 5;
        if (direction > 180) direction -= 360;
        allVecs[i] = Vector(Vector::AngMag {}, direction, strength);
    }

    Vector *strongestVectors[AVERAGED_VECTOR_MAX];
    for (int i = 0; i < AVERAGED_VECTOR_MAX; i++) {
        for (int j = 0; j < 10; j++) {
            if (allVecs[j].length > strongestVectors[i]-> length) {
                strongestVectors[i] = &allVecs[j];
                allVecs[j] = Vector();
            }
        }
    }

    signalVec = Vector();
    for (int i = 0; i < AVERAGED_VECTOR_MAX; i++) {
        signalVec = signalVec + *strongestVectors[i];
    }
    signalVec.length = strongestVectors[0]-> length;
}

float QikEasy::strengthToDistance(const uint16_t &strength) {
    return 0.0f;
}
