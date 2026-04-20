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
