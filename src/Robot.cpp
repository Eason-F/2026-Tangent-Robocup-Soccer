#include <Robot.hpp>

Button::Button(const int &pin): buttonPin(pin) {}

void Button::setup() {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool Button::isPressed() {
    return !digitalRead(buttonPin);
}

Robot::Robot() : button(41), irSensor(Wire2), imu(Wire2), colourSensor() {}

void Robot::setup() {
    button.setup();
    colourSensor.setup();
    drive.setup();
    irSensor.setup();
    odometry.setup();
    imu.setup(); imu.resetYawOrigin();
}

void Robot::run() {
    colourSensor.update(elapsedLastTime);
    odometry.update();
    irSensor.updateReadings();
    imu.updateReadings();

    if (button.isPressed()) {
        if (elapsedLastTime >= LOOP_TIME_MS) {
            float dt = elapsedLastTime / 1000.0f;
            elapsedLastTime = 0;

            conditionallyBreakLoop(drive.correctHeading(dt, imu.getRelativeYaw()));
            conditionallyBreakLoop(handleEdgeDetection(dt));

            drive.moveInDirection(dt, irSensor.getDirectionDegrees(), MOVE_SPEED);
        }
    } else {
        drive.stop();
        imu.resetYawOrigin();
    }

    LOG("IRDir", irSensor.getDirectionDegrees());
    LOG("ColourAngleDeg", colourSensor.getDirectionDegrees());
    LOG_NEXT;
    // LOG("IMU", imu.getRelativeYaw()); LOG("Heading Corrected", drive.headingCorrected(imu.getRelativeYaw())); LOG_NEXT;
    // LOG("Moving direction:", movedir); LOG_NEXT;    
    // LOG("Yaw:", heading); LOG_NEXT;  
    // LOG("Heading correction:", heading); LOG_NEXT;
}


bool Robot::handleEdgeDetection(float dt) {
    if (!colourSensor.detectedEdge()) {
        return false;
    }

    const float colourDirection = colourSensor.getDirectionDegrees();
    const float moveAwayDirection = colourDirection + 180.0f;

    drive.stop();
    drive.moveInDirection(dt, moveAwayDirection, BACK_SPEED);
    delay(500);

    LOG_PRINT("Colour sensor detected HIGH"); 
    LOG("Moving back in direction:", moveAwayDirection); LOG_NEXT;
    return true;
}
