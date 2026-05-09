#include <Robot.hpp>

void Robot::run() {
    drive.moveInDirection(270, 0);
    odometry.updatePosition();
    LOG_PRINT("X", odometry.getX()); LOG_PRINT("Y", odometry.getY()); LOG_NEXT;
}