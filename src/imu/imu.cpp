#include <imu/imu.hpp>

IMU::IMU(TwoWire &wirePort, const uint8_t &address)
    : wirePort(&wirePort), bno(SENSOR_ID, address, &wirePort) {}

bool IMU::setup() {
    wirePort->begin();
    wirePort->setClock(I2C_CLOCK);

    connected = bno.begin();
    if (!connected) {
        Serial.println("BNO055 not detected");
        return false;
    }

    delay(50);
    bno.setExtCrystalUse(true);
    update();

    return true;
}

void IMU::update() {
    if (!connected) {
        return;
    }

    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    yaw = euler.x();
}

float IMU::getYaw() {
    update();
    return yaw;
}

bool IMU::isConnected() {
    return connected;
}

void IMU::getCalibration(uint8_t &system, uint8_t &gyro, uint8_t &accel, uint8_t &mag) {
    if (!connected) {
        system = 0;
        gyro = 0;
        accel = 0;
        mag = 0;
        return;
    }

    bno.getCalibration(&system, &gyro, &accel, &mag);
}
