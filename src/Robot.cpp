#include <Robot.hpp>

Button::Button(const int &pin): buttonPin(pin) {}

void Button::setup() {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool Button::isPressed() {
    return !digitalRead(buttonPin);
}

void Robot::setup() {
    button.setup();
    drive.setup();
}

void Robot::run() {
    // LOG("button", button.isPressed()); LOG_NEXT;
    if (button.isPressed()) {
        unsigned long now = millis();
        if (now - lastTime >= LOOP_TIME_MS) {
            float dt = (now - lastTime) / 1000.0f;
            lastTime = now;

            drive.moveInDirection(dt, 0, 1000);
        }
    } else {
        drive.stop();
    }
}