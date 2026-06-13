#include <imu/imu.hpp>

IMU::IMU(TwoWire &wirePort)
    : wirePort(wirePort), sensor(SENSOR_ID, BNO055_ADDRESS_A, &wirePort) {}

void IMU::setup() {
    wirePort.begin();
    wirePort.setClock(I2C_CLOCK);

    connected = sensor.begin();
    if (!connected) {
        Serial.println("BNO055 not detected");
    }

    delay(50);
    sensor.setExtCrystalUse(true);
    updateReadings();
}

void IMU::updateReadings() {
    if (!connected) {
        return;
    }

    imu::Vector<3> vector = sensor.getVector(Adafruit_BNO055::VECTOR_EULER);
    yaw = vector.x();
    if (yaw > 180) {
        yaw -= 360;
    }
}

float IMU::getYaw() {
    updateReadings();
    return yaw;
}

void IMU::getCalibration(uint8_t &system, uint8_t &gyro, uint8_t &accel, uint8_t &mag) {
    if (!connected) {
        system = gyro = accel = mag = 0;
        return;
    }

    sensor.getCalibration(&system, &gyro, &accel, &mag);
}
