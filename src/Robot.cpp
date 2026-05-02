#include <Robot.hpp>

void Robot::run() {
    unsigned long now = millis();
    float dt = (now - lastTime) / 1000.0f;
    if (now - lastTime >= LOOP_TIME_MS) {
        drive.motor1.setMotorRPM(200, dt);
    }

    odometry.updatePosition();
    LOG_PRINT("X", odometry.getX()); LOG_PRINT("Y", odometry.getY()); LOG_NEXT;
}