#include <imu/imu.hpp>

IMU::IMU(TwoWire &wirePort)
    : wirePort(wirePort), bno(SENSOR_ID, BNO055_ADDRESS_A, &wirePort) {}

void IMU::setup() {
    wirePort.begin();
    wirePort.setClock(I2C_CLOCK);

    if (!bno.begin()) {
        Serial.println("BNO055 not detected");
    }

    bno.setExtCrystalUse(true);
    updateReadings();
}

void IMU::getCalibration(uint8_t &system, uint8_t &gyro, uint8_t &accel, uint8_t &mag) {
    bno.getCalibration(&system, &gyro, &accel, &mag);
    //do something with data
}

void IMU::resetOffsets() {
    headingOffset = yaw;
}

void IMU::updateReadings() {
    imu::Vector<3> vector = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    yaw = vector.x();
    roll = vector.y();
    pitch = vector.z();
}

float IMU::getYaw() {
    float actual = yaw - headingOffset;
    return (actual > 180) ? actual - 360 : actual;
}
