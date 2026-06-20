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
    imu.resetYawOrigin();
}

bool Robot::handleColourSensor() {
    if (!colourSensor.detectedEdge()) {
        return false;
    }

    drive.stop();
    LOG("Colour sensor detected HIGH", true); LOG_NEXT;
    delay(1000);
    movedir = lastDirection * 20 - 180; 
    LOG("Moving back in direction:", movedir); LOG_NEXT;
    drive.moveInDirection(0.5, movedir, BACK_SPEED);
    delay(500);
    drive.stop();
    delay(10);
    return true;
}

void Robot::run() {
    irSensor.updateReadings();
    imu.updateReadings();
    if (button.isPressed()) {
        unsigned long now = millis();
        if (now - lastTime >= LOOP_TIME_MS) {
            if (handleColourSensor()) return;

            float dt = (now - lastTime) / 1000.0f;
            lastTime = now;
            irSensor.updateReadings();
            if (handleHeadingAdjustment(dt)) {
                if (handleColourSensor()) return;
                return;
            }
            movedir = qikeasyDirection*20;
            drive.moveInDirection(dt, movedir, MOVE_SPEED);
        lastDirection = qikeasyDirection;  
        if (handleColourSensor()) return;

        }
            
    } else {
        drive.stop();
        imu.resetYawOrigin();
    }

    // LOG("IR Direction", qikeasyDirection); LOG_NEXT;
    // LOG("IR Strength", qikeasyStrength); LOG_NEXT;
    // LOG("IMU", imu.getYaw()); LOG_NEXT;
    // LOG("colour", colourSensor.sensorState()); LOG_NEXT;
    // LOG("Moving direction:", movedir); LOG_NEXT;    
    // LOG("Yaw:", heading); LOG_NEXT;  
    // LOG("Heading correction:", heading); LOG_NEXT;
}
