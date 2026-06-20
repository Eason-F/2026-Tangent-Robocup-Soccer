#include <Robot.hpp>

Button::Button(const int &pin): buttonPin(pin) {}

void Button::setup() {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool Button::isPressed() {
    return !digitalRead(buttonPin);
}

Robot::Robot() : button(41), irSensor(Wire2), imu(Wire2), colourSensor(22) {}

void Robot::setup() {
    button.setup();
    drive.setup();
    irSensor.setup();
    imu.setup(); imu.resetYawOrigin();
}

void Robot::run() {
    unsigned long now = millis();
    colourSensor.update(now - lastTime);
    irSensor.updateReadings();
    imu.updateReadings();

    if (button.isPressed()) {
        conditionallyBreakLoop(handleEdgeDetection());
        if (now - lastTime >= LOOP_TIME_MS) {
            float dt = (now - lastTime) / 1000.0f;
            lastTime = now;
            drive.moveInDirection(dt, 0, MOVE_SPEED);
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


bool Robot::handleEdgeDetection() {
    if (!colourSensor.detectedEdge()) {
        return false;
    }

    drive.stop();
    drive.moveInDirection(0.5, drive.lastDirection - 180, BACK_SPEED);
    delay(500);

    LOG_PRINT("Colour sensor detected HIGH"); 
    LOG("Moving back in direction:", drive.lastDirection - 180); LOG_NEXT;
    return true;
}
