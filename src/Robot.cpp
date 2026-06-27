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
    return false;
    if (!colourSensor.detectedEdge()) {
        return false;
    }
    drive.stop();
    int movedir = lastDirection - 180; 
    LOG("Moving back in direction:", movedir); LOG_NEXT;
    drive.moveInDirection(0.5, movedir, backspd);
    delay(300);
    lastDirection = movedir;
    return true;    
}

bool Robot::handleHeadingAdjustment(float dt) {
    if (heading <= targetHeading + GYRO_RANGE && heading >= targetHeading - GYRO_RANGE) {
        return false;
    }

    int adjustmentRate = -heading * GYRO_SPD_MULT;
    if (adjustmentRate > 0) {
        adjustmentRate += GYRO_SPD;
    } else {
        adjustmentRate -= GYRO_SPD;
    }
    LOG("Adjusting rate:", adjustmentRate); LOG_NEXT;   
    drive.turnInDirection(dt, adjustmentRate);
    return true;
}

void Robot::run() {
    irSensor.qikeasyReading(qikeasyDirection, qikeasyStrength);
    heading = imu.getRelativeYaw();
    if (button.isPressed()) {
        if (handleColourSensor()) return;

        unsigned long now = millis();
        if (false) {
            justOn = false;
            drive.moveInDirection(0.5, 0, 400);
            delay(1000);
            return;
        }
        if (now - lastTime >= LOOP_TIME_MS) {
            float dt = (now - lastTime) / 1000.0f;
            lastTime = now;
            irSensor.updateReadings();

            qikeasyDirection = qikeasyDirection * 20; // convert to degrees
            qikeasyDirection -= 180; // convert to robot frame of reference
            int movedir = drive.moveAroundBall(qikeasyDirection, qikeasyStrength);
            LOG("IR Direction", qikeasyDirection); LOG_NEXT;
            LOG("Moving in direction:", movedir); LOG_NEXT;
            LOG("Strength", qikeasyStrength); LOG_NEXT;
            if (movedir == -1) {
                drive.stop();
                return;
            } else {
                drive.moveInDirection(dt, movedir, movespd);
            }
        if (handleHeadingAdjustment(dt)) {
            if (handleColourSensor()) return;
            return;
        }
        lastDirection = degrees(irSensor.signalVec.angle);    
        }       
    } else {
        drive.stop();
        imu.resetYawOrigin();
        heading = 0.0f;
        justOn = true;
    }
    // LOG("IR Direction", qikeasyDirection); LOG_NEXT;
    // LOG("IR Strength", qikeasyStrength); LOG_NEXT;
    // LOG("IMU", imu.getYaw()); LOG_NEXT;
    // LOG("colour", colourSensor.sensorState()); LOG_NEXT;
}
