#include <Arduino.h>

#include <drive/Drive.hpp>
#include <odometry/Odometry.hpp>
#include <colour/colour.hpp>
#include <util/util.hpp>
#include <util/button.hpp>

class Robot {
    public:
        Button startButton;
        // Drive drive;
        ColourSensor colourSensor = ColourSensor(20);
        // OpticalOdometry odometry = OpticalOdometry(Wire);
        
        void run();

    private:
};