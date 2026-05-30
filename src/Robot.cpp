#include <Robot.hpp>

void Robot::setup() {
    drive.setup();
}

void Robot::run() {
    unsigned long now = millis();
    if (now - lastTime >= LOOP_TIME_MS) {
        float dt = (now - lastTime) / 1000.0f;
        lastTime = now;

        drive.moveInDirection(dt, 0, 1000);
        LOG("rpm", drive.motor1.angularVelocityRPM); LOG_NEXT;
    }
}