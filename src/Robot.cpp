#include <Robot.hpp>

void Robot::run() {
    unsigned long now = millis();
    // if (now - lastTime >= LOOP_TIME_MS) {
    //     float dt = (now - lastTime) / 1000.0f;
    //     lastTime = now;

    //     drive.motor1.setMotorRPM(100, dt);
    //     LOG_PRINT("rpm", drive.motor1.angularVelocityRPM); LOG_NEXT;
    // }
    drive.motor1.setMotorDutyCycle(100);
}