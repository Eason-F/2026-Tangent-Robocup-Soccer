#include <Arduino.h>

#include <drive/Drive.hpp>
#include <odometry/Odometry.hpp>
#include <qikeasy/QikEasy.hpp>
#include <util/button.hpp>

class Robot {
    public:
        Button startButton;
        Drive drive;
        // OpticalOdometry odometry = OpticalOdometry(Wire);
        // QikEasy irSensor = QikEasy(Wire);
        
        void run();

    private:
};