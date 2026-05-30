#include <Arduino.h>

#include <drive/Drive.hpp>
#include <odometry/Odometry.hpp>
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
        Button button = Button(41);
        Drive drive;
        // ColourSensor colourSensor = ColourSensor(20);
        // OpticalOdometry odometry = OpticalOdometry(Wire);
        
        void setup();
        void run();

    private:
        static constexpr uint LOOP_TIME_MS = 100;
        unsigned long lastTime = millis();
};