#include <Arduino.h>

#include <drive/Drive.hpp>
#include <odometry/Odometry.hpp>
#include <util/button.hpp>

class Robot {
    public:
        Button startButton;
        Drive drive;
        OpticalOdometry odometry = OpticalOdometry(Wire);
        
        void run();

    private:
};