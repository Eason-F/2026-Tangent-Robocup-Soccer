#include <qikeasy/QikEasy.hpp>

QikEasy::QikEasy(TwoWire &wirePort) : wirePort(wirePort) {}

void QikEasy::setup() {
    wirePort.begin();
}

void QikEasy::readMeasuredSignal(const uint8_t &moduleIndex, uint8_t &direction, uint16_t &strength) {
    const uint8_t moduleAddress = MODULE_ADDRESSES[moduleIndex % 2];

    wirePort.beginTransmission(moduleAddress);
    wirePort.write(DIRECTION_REGISTER);
    wirePort.endTransmission(false);

    wirePort.requestFrom(moduleAddress, (uint8_t) 1);
    if (wirePort.available() >= 1) {
        direction = wirePort.read();
    }

    wirePort.beginTransmission(moduleAddress);
    wirePort.write(STRENGTH_REGISTER);
    wirePort.endTransmission(false);

    wirePort.requestFrom(moduleAddress, (uint8_t) 2);
    if (wirePort.available() >= 2) {
        strength = wirePort.read() << 8;
        strength |= wirePort.read();
    }
}

void QikEasy::qikeasyReading(uint8_t &direction, uint16_t &strength) {
    uint16_t strength1 = 0;
    uint8_t direction1 = 0;
    uint16_t strength2 = 0;
    uint8_t direction2 = 0;

    readMeasuredSignal(0, direction1, strength1);
    readMeasuredSignal(1, direction2, strength2);

    if (strength1 > strength2) {
        if (direction1 > 5) {
            direction1 -= 5;
        } else {
            direction1 += 13;
        }

        direction = direction1;
        strength = strength1;
    } else if (strength2 > strength1) {
        direction2 += 4;

        direction = direction2;
        strength = strength2;
    }
}

uint16_t QikEasy::signalStrength(const int &receiverID) {
    const uint8_t moduleAddress = MODULE_ADDRESSES[(receiverID / 5) % 2];
    // LOG("address", moduleAddress);
    // LOG("register", RECEIVER_REGISTERS[receiverID % 5]);

    wirePort.beginTransmission(moduleAddress);
    wirePort.write(RECEIVER_REGISTERS[receiverID % 5]);
    wirePort.endTransmission(false);
    
    uint16_t strength = 0;
    wirePort.requestFrom(moduleAddress, (uint8_t) 2);
    if (wirePort.available() >= 2) {
        strength = wirePort.read() << 8;
        strength |= wirePort.read();
    }
    // LOG(receiverID - 2, strength);
    return strength;
}

void QikEasy::updateReadings() {
    for (int i = 2; i < 12; i++) {
        uint16_t strength = signalStrength(i);
        float direction = ((i - 2) * PI / 5);
        allVecs[i - 2] = Vector(Vector::AngMag {}, direction, strength);
        // LOG("d", allVecs[i - 2].angle);
    }

    Vector strongestVectors[AVERAGED_VECTOR_MAX];
    for (int i = 0; i < AVERAGED_VECTOR_MAX; i++) {
        for (int j = 0; j < 10; j++) {
            if (allVecs[j].length > strongestVectors[i].length) {
                strongestVectors[i] = allVecs[j];
                allVecs[j] = Vector();
            }
        }
    }

    signalVec = Vector();
    for (int i = 0; i < AVERAGED_VECTOR_MAX; i++) {
        signalVec = signalVec + strongestVectors[i];
    }
    signalVec.length = strongestVectors[0].length;
}

float QikEasy::strengthToDistance(const uint16_t &strength) {
    return 0.0f;
}
