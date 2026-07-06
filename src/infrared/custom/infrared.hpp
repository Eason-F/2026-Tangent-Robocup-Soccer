#include <Arduino.h>
#include <infrared/InfraredABC.hpp>

class IRSensor : public InfraredABC{
    public:
        IRSensor(uint8_t &rxPin, uint8_t &txPin);
        void setup() override;

        void updateReadings() override;
        float getDirectionDegrees() override;
        float getSignalStrength() override;

    private:
        const uint8_t rxPin, txPin;

};