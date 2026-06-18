#include <Arduino.h>

#include <drive/Drive.hpp>
#include <odometry/Odometry.hpp>
#include <qikeasy/QikEasy.hpp>
#include <imu/imu.hpp>
#include <util/util.hpp>
#include <colour/colour.hpp>


class Button {
    private:
        const int buttonPin;

    public:
        Button(const int &pin);
        void setup();

        bool isPressed();
};

class Robot {
    public:
        Robot();

        void setup();
        void run();

    private:
        static constexpr uint LOOP_TIME_MS = 50;
        static constexpr int GYRO_RANGE = 25;
        static constexpr int GYRO_SPD = 20;
        static constexpr float GYRO_SPD_MULT = 0.5f;

        unsigned long lastTime = millis();
        unsigned int lastDirection = 0;

        uint8_t qikeasyDirection = 0;
        uint16_t qikeasyStrength = 0;

        int movedir = 0;
        uint16_t movespd = 200;
        uint16_t backspd = 150;
        float heading = 0.0f;
        float targetHeading = 0.0f;

        bool handleColourSensor();
        bool handleHeadingAdjustment(float dt);

        bool justOn = false;
        Button button;
        QikEasy irSensor;
        Drive drive;
        IMU imu;
        ColourSensor colourSensor;
        // OpticalOdometry odometry = OpticalOdometry(Wire);
};
