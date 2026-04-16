#include <Robot.hpp>

void Robot::run() {
    odometry.updatePosition();
    Serial.print("X: "); Serial.print(odometry.getX()); Serial.print("    "); Serial.print("Y: "); Serial.println(odometry.getY());
}