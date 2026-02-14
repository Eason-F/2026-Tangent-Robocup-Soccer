enum MotorDirection {
    ANTICLOCKWISE,
    CLOCKWISE
};

class Motor {
    public:
        const static int PULSE_PER_REVOLUTION = 18;

        int angularVelocityRPM;
        volatile int pulseCount;

        Motor(const int pwmPin, const int directionPin, const int pulsePin);

        void incrementPulseCount();

        void setMotorSpeed(int speed, enum MotorDirection direction);

    private: 
        int pwmPin;
        int directionPin;
        int pulsePin;
};