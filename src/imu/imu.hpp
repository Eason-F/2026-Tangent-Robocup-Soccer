#pragma once

#include <Adafruit_BNO055.h>
#include <Arduino.h>
#include <Wire.h>
#include <utility/imumaths.h>

class IMU {
    public:
        IMU(TwoWire &wirePort, const uint8_t &address = 0x28);

        bool setup();
        void update();

        float getYaw();
        float getRelativeYaw();
        void resetYawOrigin();
        bool isConnected();
        void getCalibration(uint8_t &system, uint8_t &gyro, uint8_t &accel, uint8_t &mag);

    private:
        static constexpr int32_t SENSOR_ID = 55;
        static constexpr uint32_t I2C_CLOCK = 400000;

        TwoWire *wirePort;
        Adafruit_BNO055 bno;
        float yaw = 0.0f;
        float yawOrigin = 0.0f;
        bool connected = false;

        static float normaliseYaw(float value);
};
