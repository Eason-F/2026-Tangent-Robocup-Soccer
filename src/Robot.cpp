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
        if (justOn) {
            justOn = false;
            drive.moveInDirection(0.5, 0, 400);
            delay(1000);
            return;
        }
        if (colourSensor.detectedEdge()) {
            drive.stop();
            LOG("Colour sensor detected HIGH", true); LOG_NEXT;
            delay(3000);

            // drive.moveInDirection(0.5, degrees(irSensor.signalVec.angle)-180, 80);
            // LOG("IR Heading:", degrees(irSensor.signalVec.angle)); LOG_NEXT;    
            // delay(300);
        } else if (now - lastTime >= LOOP_TIME_MS) {
            float dt = (now - lastTime) / 1000.0f;
            lastTime = now;
            // drive.moveInDirection(dt, 0, 500);
            drive.moveInDirection(dt, degrees(irSensor.signalVec.angle), 200);
            lastDirection = degrees(irSensor.signalVec.angle);
            // LOG("IR Heading:", degrees(irSensor.signalVec.angle)); LOG_NEXT;    

        }
            
    } else {
        drive.stop();
        justOn = true;
    }
    // LOG("IR", degrees(irSensor.signalVec.angle)); LOG_NEXT;
    // LOG("IMU", imu.getYaw()); LOG_NEXT;
}