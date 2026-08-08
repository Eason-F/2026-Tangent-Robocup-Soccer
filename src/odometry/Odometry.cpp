#include <odometry/Odometry.hpp>
#include <util/util.hpp>

OpticalOdometry::OpticalOdometry(TwoWire &wirePort) : wirePort(wirePort) {}

void OpticalOdometry::setup() {
    wirePort.begin();
    wirePort.setClock(1000000);
    while (!odometrySensor.begin(wirePort)) {
        LOG_PRINT("Disconnected odometry"); LOG_NEXT;
    };
    odometrySensor.calibrateImu();
    odometrySensor.resetTracking();

    odometrySensor.setLinearUnit(kSfeOtosLinearUnitMeters);
    odometrySensor.setAngularUnit(kSfeOtosAngularUnitDegrees);
}

float OpticalOdometry::getX() {
    return position.x * LINEAR_MULTIPLIER;
}

float OpticalOdometry::getY() {
    return position.y * LINEAR_MULTIPLIER;
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
