#include <Arduino.h>

class Motor {
    public:
        const static int PULSE_PER_REVOLUTION = 18;
        
        int pwmPin;
        int directionPin;
        int pulsePin;

        int angularVelocityRPM;
        volatile int pulseCount;

        Motor(const int pwmPin, const int directionPin, const int pulsePin);

        void incrementPulseCount();

        void setMotorSpeed(int speed, MotorDirection direction);
};

enum MotorDirection {
    ANTICLOCKWISE,
    CLOCKWISE
};