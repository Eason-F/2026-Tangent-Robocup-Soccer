#include <odometry/Odometry.hpp>

OpticalOdometry::OpticalOdometry(TwoWire &wirePort) : wirePort(wirePort) {}

void OpticalOdometry::setup() {
    wirePort.begin();
    while (!odometrySensor.begin(wirePort)) {
        LOG_PRINT("Disconnected odometry"); LOG_NEXT;
    };
    odometrySensor.calibrateImu();
    odometrySensor.resetTracking();

    odometrySensor.setLinearScalar(-1.0);
    odometrySensor.setLinearUnit(kSfeOtosLinearUnitMeters);
    odometrySensor.setAngularUnit(kSfeOtosAngularUnitDegrees);
}

float OpticalOdometry::getX() {
    return position.x;
}

float OpticalOdometry::getY() {
    return position.y;
}

float OpticalOdometry::getHeading() {
    return position.h;
}

void OpticalOdometry::update() {
    odometrySensor.getPosition(position);
}

void OpticalOdometry::setPosition(sfe_otos_pose2d_t &pose) {
    odometrySensor.resetTracking();
    odometrySensor.setPosition(pose);
}

void OpticalOdometry::resetPosition() {
    odometrySensor.calibrateImu(255, true);
    odometrySensor.resetTracking();
}