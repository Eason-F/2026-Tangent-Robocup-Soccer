#include <Robot.hpp>

void Robot::run() {
    odometry.updatePosition();
    LOG_PRINT("X", odometry.getX()); LOG_PRINT("Y", odometry.getY()); LOG_NEXT;
}