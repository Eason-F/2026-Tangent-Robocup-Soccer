#include <imu/imu.hpp>

IMU::IMU(TwoWire &wirePort)
    : wirePort(wirePort), bno(SENSOR_ID, BNO055_ADDRESS_A, &wirePort) {}

bool IMU::setup() {
    wirePort.begin();
    wirePort.setClock(I2C_CLOCK);

    if (!bno.begin()) {
        Serial.println("BNO055 not detected");
        return false;
    }

    delay(50);
    bno.setExtCrystalUse(true);
    resetYawOrigin(); updateReadings();
    return true;
}

void IMU::updateReadings() {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    yaw = normaliseYaw(euler.x());
}

float IMU::getYaw() {
    updateReadings();
    return yaw;
}

float IMU::getRelativeYaw() {
    return normaliseYaw(yaw - yawOrigin);
}

void IMU::resetYawOrigin() {
    yawOrigin = yaw;
}

float IMU::normaliseYaw(float value) {
    while (value > 180.0f) {
        value -= 360.0f;
    }

    while (value < -180.0f) {
        value += 360.0f;
    }

    return value;
}
