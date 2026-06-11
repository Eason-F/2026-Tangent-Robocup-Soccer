#include <imu/imu.hpp>

IMU::IMU(TwoWire &wirePort)
    : wirePort(wirePort), bno(SENSOR_ID, BNO055_ADDRESS_A, &wirePort) {}

bool IMU::setup() {
    wirePort.begin();
    wirePort.setClock(I2C_CLOCK);

    connected = bno.begin();
    if (!connected) {
        Serial.println("BNO055 not detected");
        return false;
    }

    delay(50);
    bno.setExtCrystalUse(true);
    updateReadings();
    return true;
}

void IMU::updateReadings() {
    if (!connected) {
        return;
    }

    imu::Vector<3> vector = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    yaw = vector.x();
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

    bno.getCalibration(&system, &gyro, &accel, &mag);
}
