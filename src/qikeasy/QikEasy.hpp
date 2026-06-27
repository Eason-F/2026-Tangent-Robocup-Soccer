#pragma once

#include <Wire.h>
#include <util/util.hpp>

class QikEasy {
    public:
        Vector signalVec;
        Vector allVecs[10];

        QikEasy(TwoWire &wirePort);
        void setup();

        void updateReadings();
        void readMeasuredSignal(const uint8_t &moduleIndex, uint8_t &direction, uint16_t &strength);
        void qikeasyReading(uint16_t &direction, uint16_t &strength);

    private:
        TwoWire &wirePort;

        static constexpr uint8_t MODULE_ADDRESSES[2] = {0x14, 0x15};
        static constexpr uint8_t RECEIVER_REGISTERS[5] = {0x4A, 0x4C, 0x4E, 0x50, 0x52};
        static constexpr uint8_t DIRECTION_REGISTER = 0x49;
        static constexpr uint8_t STRENGTH_REGISTER = 0x54;

        uint16_t signalStrength(const int &receiverID);
        static float strengthToDistance(const uint16_t &strength);

        static constexpr uint8_t AVERAGED_VECTOR_MAX = 3;
};
