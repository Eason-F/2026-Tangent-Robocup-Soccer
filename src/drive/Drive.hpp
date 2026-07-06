#pragma once

#include <drive/motor/Motor.hpp>
#include <odometry/Odometry.hpp>
#include <util/util.hpp>

#define ENCODER_USE_INTERRUPTS

class Drive {
    public:
        static constexpr float motorKP = 0.2;
        static constexpr float motorKI = 0;
        static constexpr float motorKD = 0;
        static constexpr float motorMax = 255;
        static constexpr float motorMin = 0;

        PIDController motorPID1 = PIDController(motorKP, motorKI, motorKD, motorMin, motorMax);
        PIDController motorPID2 = PIDController(motorKP, motorKI, motorKD, motorMin, motorMax);
        PIDController motorPID3 = PIDController(motorKP, motorKI, motorKD, motorMin, motorMax);
        PIDController motorPID4 = PIDController(motorKP, motorKI, motorKD, motorMin, motorMax);

        static constexpr float positionKP = 0.2;
        static constexpr float positionKI = 0;
        static constexpr float positionKD = 0;
        static constexpr float positionMax = Motor::MAX_RPM;
        static constexpr float positionMin = -Motor::MAX_RPM;
        PIDController positionPID = PIDController(positionKP, positionKI, positionKD, positionMin, positionMax);

        Motor motor1;
        Motor motor2;
        Motor motor3;
        Motor motor4;

        Drive();
        void setup();

        void updateRPM(const float &dt);
        bool correctHeading(const float &dt, float heading);
        bool headingCorrected(float heading);
        
        void moveInDirection(const float &dt, int directionDegrees, int rpm);
        void moveToPoint(const float &dt, const sfe_otos_pose2d_t &target, OpticalOdometry &odometry);
        void moveToPoint(const float &dt, const float &targetX, const float &targetY, OpticalOdometry &odometry);
        void turnInDirection(const float &dt, int rpm);
        void stop();

        float lastDirection;

    private: 
        static constexpr int DIRECTION_PIN1_1 = 2;
        static constexpr int DIRECTION_PIN2_1 = 3;
        static constexpr int ENCODER_PIN1_1 = 33;
        static constexpr int ENCODER_PIN2_1 = 34;

        static constexpr int DIRECTION_PIN1_2 = 4;
        static constexpr int DIRECTION_PIN2_2 = 5;
        static constexpr int ENCODER_PIN1_2 = 35;
        static constexpr int ENCODER_PIN2_2 = 36;

        static constexpr int DIRECTION_PIN1_3 = 6;
        static constexpr int DIRECTION_PIN2_3 = 7;
        static constexpr int ENCODER_PIN1_3 = 37;
        static constexpr int ENCODER_PIN2_3 = 38;

        static constexpr int DIRECTION_PIN1_4 = 8;
        static constexpr int DIRECTION_PIN2_4 = 9;
        static constexpr int ENCODER_PIN1_4 = 39;
        static constexpr int ENCODER_PIN2_4 = 40;

        static constexpr float HEADING_ADJUSTMENT_MULTIPLIER = 0.5f;
        static constexpr int HEADING_TOLERANCE_DEGREES = 15;
        static constexpr int TURN_SPEED = 30;
        float targetHeading = 0;
};
