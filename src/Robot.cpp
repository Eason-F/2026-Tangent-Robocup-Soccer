#include <Robot.hpp>

Button::Button(const int &pin): buttonPin(pin) {}

void Button::setup() {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool Button::isPressed() {
    return !digitalRead(buttonPin);
}

Robot::Robot() : button(41), irSensor(Wire2), imu(Wire2), odometry(Wire), colourSensor(22){}

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

            // conditionallyBreakLoop(drive.correctHeading(dt, imu.getRelativeYaw()));
            // conditionallyBreakLoop(handleEdgeDetection(dt));

            // drive.moveInDirection(dt, irSensor.getDirectionDegrees(), MOVE_SPEED);
            drive.moveToPoint(dt, MOVE_SPEED, 0.1, 0.5, odometry);
        }
    } else {
        drive.stop();
        imu.resetYawOrigin();
        odometry.resetPosition();
    }

    // LOG("odometryX", odometry.getX()); LOG("odometryY", odometry.getY()); LOG("odometryH", odometry.getHeading());
    // LOG("IRDir", irSensor.getDirectionDegrees());
    // LOG("ColourAngleDeg", colourSensor.getDirectionDegrees());
    // LOG("IMU", imu.getRelativeYaw()); LOG("Heading Corrected", drive.headingCorrected(imu.getRelativeYaw())); LOG_NEXT;
    // LOG("Moving direction:", movedir); LOG_NEXT;    
    // LOG("Heading correction:", heading); LOG_NEXT;
    LOG_NEXT;
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

    LOG("\n\nMoving back in direction:", drive.lastDirection - 180); LOG_NEXT;
    return true;
}
