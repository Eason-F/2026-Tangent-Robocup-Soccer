#include <Robot.hpp>

Button::Button(const int &pin): buttonPin(pin) {}

void Button::setup() {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool Button::isPressed() {
    return !digitalRead(buttonPin);
}

Robot::Robot() : button(41), irSensor(Serial1), imu(Wire2), odometry(Wire), colourSensor(22){}

void Robot::setup() {
    button.setup();
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

    // LOG("irDirection", irSensor.getDirectionDegrees()); 
    // LOG("heading", imu.getRelativeYaw()); 
    // LOG("colour", colourSensor.sensorState()); 
    LOG("odometryX", odometry.getX()); LOG("odometryY", odometry.getY()); LOG("odometryH", odometry.getHeading());
    // LOG("rpm", drive.motor1.angularVelocityRPM);
    LOG_NEXT;
}


bool Robot::handleEdgeDetection(float dt) {
    if (!colourSensor.detectedEdge()) {
        return false;
    }

    drive.stop();
    drive.moveInDirection(dt, drive.lastDirection - 180, BACK_SPEED);
    delay(500);

    LOG("\n\nMoving back in direction:", drive.lastDirection - 180); LOG_NEXT;
    return true;
}
