#include <Arduino.h>

#include <drive/Drive.hpp>
// #include <odometry/Odometry.hpp>
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
        static constexpr uint LOOP_TIME_MS = 10;
        static constexpr uint MOVE_SPEED = 250;
        static constexpr uint BACK_SPEED = 200;

        static constexpr uint8_t ESCAPE_DURATION = 500;
        float escapeDirection = 0.0f;
        elapsedMillis elapsedEscapeTime = ESCAPE_DURATION;

        elapsedMillis elapsedLastTime;

        bool handleEdgeDetection(float dt);

        Button button;
        QikEasy irSensor;
        Drive drive;
        IMU imu;
        // OpticalOdometry odometry;
        ColourSensor colourSensor;
};
