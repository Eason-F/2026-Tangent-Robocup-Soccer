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
      colourSensor(22),
      logger(Serial, LOG_INTERVAL_MS) {
    if (WITH_ODOMETRY) {
        odometry = std::make_unique<OpticalOdometry>(Wire2);
    }
}

void Robot::setup() {
    button.setup();
    colourSensor.setup();
    drive.setup();
    irSensor.setup();
    imu.setup(); imu.resetYawOrigin();
    if (odometry != nullptr) {
        odometry-> setup();
    }
}

void Robot::run() {
    colourSensor.update(elapsedLastTime);
    irSensor.update();
    imu.update();
    if (odometry != nullptr) {
        odometry-> update();
    }

    if (button.isPressed()) {
        if (elapsedLastTime >= LOOP_TIME_MS) {
            float dt = elapsedLastTime / 1000.0f;
            elapsedLastTime = 0;

            // conditionallyBreakLoop(handleEdgeDetection(dt));
            handleHeadingCorrection(dt, 0);
            
            maneuverAroundBall(dt, 0);
            // drive.moveInDirection(dt, irSensor.getDirectionDegrees(), 100);
            // drive.motor1.setMotorRPM(100, dt);
        }
    } else {
        drive.stop();
        imu.resetYawOrigin();
        if (odometry != nullptr) {
            odometry-> resetPosition();
        }
    }

    logger.update([this](Logger &log) {
        log.log("dir", irSensor.getDirectionDegrees());
        log.log("str", irSensor.getSignalStrength());
        log.log("movementDir", drive.lastDirection);
        log.log("state", static_cast<int>(robotState));

        // log.log("heading", imu.getRelativeYaw());
        // log.log("colour", colourSensor.sensorState());
        // log.log("odometryX", odometry.getX());
        // log.log("odometryY", odometry.getY());
        // log.log("odometryH", odometry.getHeading());
        // log.log("rpm", drive.motor1.angularVelocityRPM);
        // log.log("ballFound", irSensor.ballFound());
    });
}

bool Robot::handleEdgeDetection(float dt) {
    if (colourSensor.detectedEdge()) {
        const Vector edgeVector = colourSensor.getVector();

        if (edgeVector.magnitude > 0.1f) {
            escapeDirection = degrees(edgeVector.angle) + 180.0f;
        }
        elapsedEscapeTime = 0;
    }

    if (elapsedEscapeTime >= ESCAPE_DURATION) {
        return false;
    }
    // drive.stop();
    drive.moveInDirection(dt, escapeDirection, BACK_SPEED);
    return true;
}

void Robot::handleHeadingCorrection(const float dt, const float targetHeading) {
    float headingError = wrapAngle180(imu.getRelativeYaw() - targetHeading);
    float adjustmentRate = -headingPID.adjustmentValue(dt, headingError) * TURN_SPD;
    drive.rotationRpm = adjustmentRate;
    // Logger::queue("headingErr", headingError);
    // Logger::queue("headingAdj", adjustmentRate);
}


void Robot::maneuverAroundBall(const float dt, const float targetBallHeading) {
    checkRobotState(dt, targetBallHeading);
    switch (robotState) {
        case SEARCH: {
            if (odometry != nullptr) {
                drive.moveToPoint(dt, SEARCH_SPD, 0, 0, *odometry);
            } else {
                drive.moveInDirection(dt, 180, SEARCH_SPD);
            }
            break;
        }
        case APPROACH: {
            float speed = approachPID.adjustmentValue(dt, ORBIT_DISTANCE, irSensor.getSignalStrength()) * APPROACH_SPD;
            drive.moveInDirection(dt, irSensor.getDirectionDegrees(), speed);
            break;
        }
        case ORBIT: {
            float headingError = wrapAngle180(irSensor.getDirectionDegrees() - targetBallHeading);
            float distanceError = ORBIT_DISTANCE - irSensor.getSignalStrength();

            float approach = orbitDistancePID.adjustmentValue(dt, distanceError);
            float tangent = -orbitTangentPID.adjustmentValue(dt, headingError);

            float orbitFactor = 1.0f - min(max(0.0, distanceError) / ORBIT_DISTANCE, 1.0f);
            tangent *= orbitFactor;

            float approachSpeed = approach * ORBIT_APPROACH_SPD;
            float tangentSpeed = tangent * ORBIT_SPD;
            Vector approachVector = Vector(Vector::AngMag {}, irSensor.getDirectionRadians(), approachSpeed);
            Vector tangentVector = Vector(Vector::Position {}, sin(irSensor.getDirectionRadians()), -cos(irSensor.getDirectionRadians())) * tangentSpeed;
            Vector finalVector = tangentVector + approachVector;

            float movementAngle = degrees(finalVector.angle);
            float movementSpeed = min(finalVector.magnitude, ORBIT_SPD);
            drive.moveInDirection(dt, movementAngle, movementSpeed);
            // logger.queue("approachspd", approachSpeed);
            // logger.queue("tangentspd", tangentSpeed);
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
        accumulatedAlignedTime = 0;
        accumulatedOrbitTime = 0;
        return;
    }

    if (robotState != State::CAPTURED) {
        robotState = State::APPROACH;

        if (ORBIT_DISTANCE - irSensor.getSignalStrength() < ORBIT_ENTRY_TOLERANCE) {
            accumulatedOrbitTime += static_cast<unsigned long>(dt * 1000);

            if (accumulatedOrbitTime >= ORBIT_DEBOUNCE_MS) {
                robotState = State::ORBIT;
            }
        } else {
            accumulatedOrbitTime = 0;
        }
    }

    float headingError = abs(wrapAngle180(
        targetBallHeading - irSensor.getDirectionDegrees()
    ));

    if (robotState == State::ORBIT) {
        if (headingError > ENTER_ALIGNMENT_TOLERANCE) {
            accumulatedAlignedTime = 0;
            return;
        }

        accumulatedAlignedTime += static_cast<unsigned long>(dt * 1000);

        if (accumulatedAlignedTime >= ALIGNED_DEBOUNCE_MS && 
            irSensor.getSignalStrength() > ORBIT_DISTANCE) {
            robotState = State::CAPTURED;
        }
    } else if (robotState == State::CAPTURED) {
        if (headingError > EXIT_ALIGNMENT_TOLERANCE) {
            robotState = State::ORBIT;
        }
    }
}
