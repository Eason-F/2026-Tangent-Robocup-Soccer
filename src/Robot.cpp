#include <Robot.hpp>

Button::Button(const int &pin): buttonPin(pin) {}

void Button::setup() {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool Button::isPressed() {
    return !digitalRead(buttonPin);
}

Robot::Robot() : button(41), irSensor(Wire2), imu(Wire2, 0x28), colourSensor(22) {}

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

            drive.moveInDirection(dt, 0, 30);
            lastDirection = degrees(irSensor.signalVec.angle);
        }
        if (colourSensor.detectedEdge()) {
            LOG("Colour sensor detected HIGH", true); LOG_NEXT;
            drive.moveInDirection(0.5, 180, 30);
            delay(1500);
            // drive.moveInDirection(0.5, lastDirection-180, 1000);
        }
    } else {
        drive.stop();
    }
    // LOG("IR", degrees(irSensor.signalVec.angle)); LOG_NEXT;
    // LOG("IMU", imu.getYaw()); LOG_NEXT;
}