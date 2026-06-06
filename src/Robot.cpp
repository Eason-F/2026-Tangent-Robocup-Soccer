#include <Robot.hpp>

Button::Button(const int &pin): buttonPin(pin) {}

void Button::setup() {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool Button::isPressed() {
    return !digitalRead(buttonPin);
}

Robot::Robot() : button(41), irSensor(Wire2), imu(Wire2, 0x28) {}

void Robot::setup() {
    button.setup();
    drive.setup();
    irSensor.setup();
    imu.setup();
}

void Robot::run() {
    irSensor.updateReadings();
    if (button.isPressed()) {
        unsigned long now = millis();
        if (now - lastTime >= LOOP_TIME_MS) {
            float dt = (now - lastTime) / 1000.0f;
            lastTime = now;

            drive.moveInDirection(dt, degrees(irSensor.signalVec.angle), 1000);
        }
    } else {
        drive.stop();
    }
    // LOG("IR", degrees(irSensor.signalVec.angle)); LOG_NEXT;
    LOG("IMU", imu.getYaw()); LOG_NEXT;
}