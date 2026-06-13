#include <Robot.hpp>

Button::Button(const int &pin): buttonPin(pin) {}

void Button::setup() {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool Button::isPressed() {
    previousState = state;
    state = !digitalRead(buttonPin);
    return state;
}

bool Button::justPressed() {
    return previousState != state;
}

Robot::Robot() : button(41), irSensor(Wire2), imu(Wire2) {}

void Robot::setup() {
    button.setup();
    drive.setup();
    irSensor.setup();
    imu.setup();
}

void Robot::run() {
    irSensor.updateReadings();
    imu.updateReadings();
    if (button.justPressed()) {
    }
    
    if (button.isPressed()) {
        unsigned long now = millis();
        if (now - lastTime >= LOOP_TIME_MS) {
            float dt = (now - lastTime) / 1000.0f;
            lastTime = now;

            drive.moveInDirection(dt, irSensor.signalVec.angle, 200, imu.getYaw());
            // drive.turnInDirection(dt, 150);
        }
    } else {
        drive.stop();
        imu.resetOffsets();
    }
    LOG("IR", degrees(irSensor.signalVec.angle));
    LOG("IMU", imu.getYaw()); LOG_NEXT;
}