#include <Arduino.h>

#include <drive/Drive.hpp>
#include <odometry/Odometry.hpp>
#include <qikeasy/QikEasy.hpp>
#include <imu/imu.hpp>
#include <util/util.hpp>


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
        static constexpr uint LOOP_TIME_MS = 100;
        unsigned long lastTime = millis();

        Button button;
        QikEasy irSensor;
        Drive drive;
        IMU imu;
        // ColourSensor colourSensor = ColourSensor(20);
        // OpticalOdometry odometry = OpticalOdometry(Wire);
};