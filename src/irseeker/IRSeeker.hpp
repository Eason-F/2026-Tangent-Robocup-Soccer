#pragma once

#include <Wire.h>

class QIKEasy {
    public:
        static const int FRONT_SEEKER_ADDRESS = 0x14;
        static const int BACK_SEEKER_ADDRESS = 0x15;
        static const int DIRECTION_REGISTER = 0x49;
        static const int STRENGTH_REGISTER = 0x54;

        static const int RECEIVER1_REGISTER = 0x4A;
        static const int RECEIVER2_REGISTER = 0x4C;
        static const int RECEIVER3_REGISTER = 0x4E;
        static const int RECEIVER4_REGISTER = 0x50;
        static const int RECEIVER5_REGISTER = 0x52;

        QIKEasy(TwoWire &wirePort);

        float signalDirection;
        float signalStrength;

    private:
};