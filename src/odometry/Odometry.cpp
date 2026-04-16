#include <odometry/Odometry.hpp>

OpticalOdometry::OpticalOdometry(TwoWire &wirePort) {
    wirePort.begin();
    while (!odometrySensor.begin(wirePort)) {
        Serial.println("Disconnected odometry");
    };
    odometrySensor.calibrateImu();
    odometrySensor.resetTracking();
}

float OpticalOdometry::getX() {
    return position.x;
}

float OpticalOdometry::getY() {
    return position.y;
}

void OpticalOdometry::updatePosition() {
    odometrySensor.getPosition(position);
}

void OpticalOdometry::setPosition(sfe_otos_pose2d_t &pose) {
    odometrySensor.resetTracking();
    odometrySensor.setPosition(pose);
}