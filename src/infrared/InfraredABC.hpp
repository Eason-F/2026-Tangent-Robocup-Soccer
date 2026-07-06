#include <Arduino.h>
#include <array>

#include <util/util.hpp>

class InfraredABC {
    public:
        virtual ~InfraredABC() = default;
        virtual void setup();
        virtual void updateReadings();
        virtual float getDirectionDegrees();
        virtual float getSignalStrength();
};