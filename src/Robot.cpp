#include <Robot.hpp>

Button::Button(const int &pin): buttonPin(pin) {}

void Button::setup() {
    pinMode(buttonPin, INPUT_PULLUP);
}

bool Button::isPressed() {
    return !digitalRead(buttonPin);
}

Robot::Robot()
    : button(41),
      irSensor(Serial4),
      imu(Wire2),
      odometry(Wire),
      colourSensor(22),
      logger(Serial, LOG_INTERVAL_MS) {}

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
            
            maneuverAroundBall(dt, 0);
            // drive.motor1.setMotorRPM(60, dt);
        }
    } else {
        drive.stop();
        imu.resetYawOrigin();
        odometry.resetPosition();
    }

    logger.update([this](Logger &log) {
        log.log("dir", irSensor.getDirectionDegrees());
        log.log("str", irSensor.getSignalStrength());
        log.log("movementDir", drive.lastDirection);
        log.log("state", static_cast<int>(robotState));

        // Add or remove log.log(...) calls here to choose the telemetry fields.
        // log.log("heading", imu.getRelativeYaw());
        // log.log("colour", colourSensor.sensorState());
        // log.log("odometryX", odometry.getX());
        // log.log("odometryY", odometry.getY());
        // log.log("odometryH", odometry.getHeading());
        // log.log("rpm", drive.motor1.angularVelocityRPM);
    });
}


bool Robot::handleEdgeDetection(float dt) {
    if (!colourSensor.detectedEdge()) {
        return false;
    }

    drive.stop();
    drive.moveInDirection(dt, drive.lastDirection - 180, BOUNDARY_BACK_SPD);
    delay(500);

    Logger::queue("movingBackDirection", drive.lastDirection - 180);
    return true;
}

void Robot::maneuverAroundBall(const float dt, const float targetBallHeading) {
    checkRobotState(dt, targetBallHeading);
    switch (robotState) {
        case SEARCH: {
            drive.moveToPoint(dt, SEARCH_SPD, 0, 0, odometry);
            break;
        }
        case APPROACH: {
            float speed = approachPID.adjustmentValue(dt, ORBIT_DISTANCE, irSensor.getSignalStrength());
            drive.moveInDirection(dt, irSensor.getDirectionDegrees(), speed);
            break;
        }
        case ORBIT: {
            float headingError = wrapAngle180(targetBallHeading - irSensor.getDirectionDegrees());
            float approachSpeed = orbitDistancePID.adjustmentValue(dt, ORBIT_DISTANCE, irSensor.getSignalStrength());
            float tangentSpeed = orbitTangentPID.adjustmentValue(dt, headingError);
            float vx = approachSpeed * cos(radians(irSensor.getDirectionDegrees())) + tangentSpeed * -sin(radians(irSensor.getDirectionDegrees()));
            float vy = approachSpeed * sin(radians(irSensor.getDirectionDegrees())) + tangentSpeed * cos(radians(irSensor.getDirectionDegrees()));
            float movementAngle = degrees(atan2(vy, vx));
            float movementSpeed = min(hypot(vx, vy), ORBIT_SPD);
            drive.moveInDirection(dt, movementAngle, movementSpeed);
            break;
        }
        case CAPTURED: {
            drive.moveInDirection(dt, irSensor.getDirectionDegrees(), CAPTURED_SPD);
            break;
        }
    }
}

void Robot::checkRobotState(const float dt, const float targetBallHeading) {
    if (!irSensor.ballFound()) {
        robotState = State::SEARCH;
        accumulatedAlignedTime = 0; return;
    } 
    
    if (robotState != State::CAPTURED) {
        robotState = State::APPROACH;
        if (abs(ORBIT_DISTANCE - irSensor.getSignalStrength()) < APPROACH_DISTANCE_TOLERANCE) {
            robotState = State::ORBIT;
        }
    }
    
    float headingError = abs(wrapAngle180(targetBallHeading - irSensor.getDirectionDegrees()));
    if (robotState == State::ORBIT) {
        if (headingError > ENTER_ALIGNMENT_TOLERANCE) {
             accumulatedAlignedTime = 0; return;
        }
        accumulatedAlignedTime += static_cast<unsigned long>(dt * 1000);
        if (accumulatedAlignedTime >= ALIGNED_DEBOUNCE_MS) {
            robotState = State::CAPTURED;
        }
    } else if (robotState == State::CAPTURED) {
        if (headingError > EXIT_ALIGNMENT_TOLERANCE) {
            robotState = State::ORBIT;
        }
    }
}
