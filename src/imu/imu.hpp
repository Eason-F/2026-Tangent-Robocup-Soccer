#pragma once

#include <Adafruit_BNO055.h>
#include <Arduino.h>
#include <Wire.h>
#include <utility/imumaths.h>

class IMU {
    public:
        IMU(TwoWire &wirePort);

        bool setup();
        void updateReadings();

        float getYaw();
        float getRelativeYaw();
        void resetYawOrigin();

    private:
        static constexpr int32_t SENSOR_ID = 55;
        static constexpr uint32_t I2C_CLOCK = 400000;

        TwoWire &wirePort;
        Adafruit_BNO055 bno;

        float yaw, yawOrigin;
        static float normaliseYaw(float value);
};
