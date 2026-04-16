#include <SparkFun_Qwiic_OTOS_Arduino_Library.h>
#include <Wire.h>

class OpticalOdometry {
    public:
        OpticalOdometry(TwoWire &wirePort);

        float getX();
        float getY();
        void updatePosition();
        void resetPosition();
        void setPosition(sfe_otos_pose2d_t &pose);

    private:
        QwiicOTOS odometrySensor;
        sfe_otos_pose2d_t position;
};

