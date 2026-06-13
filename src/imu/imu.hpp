#pragma once

#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include <Arduino.h>
#include <Wire.h>

class IMU {
    public:
        IMU(TwoWire &wirePort);

        void setup();
        void updateReadings();

        void getCalibration(uint8_t &system, uint8_t &gyro, uint8_t &accel, uint8_t &mag);
        void resetOffsets();

        float getYaw();

    private:
        static constexpr int32_t SENSOR_ID = 55;
        static constexpr uint32_t I2C_CLOCK = 400000;
        static constexpr uint8_t address = 0x28;

        TwoWire &wirePort;
        Adafruit_BNO055 bno;
        float headingOffset;

        float yaw, pitch, roll = 0;
};
