#include <Arduino.h>
#include <memory>

#include <drive/Drive.hpp>
#include <odometry/Odometry.hpp>
#include <ir/uart/UartIRSensor.hpp>
#include <imu/imu.hpp>
#include <util/Logger.hpp>
#include <util/Vector.hpp>
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

enum State {
    SEARCH,
    APPROACH,
    ORBIT,
    CAPTURED
};

class Robot {
    public:
        Robot();

        void setup();
        void run();

    private:
        static constexpr bool WITH_ODOMETRY = false;

        static constexpr uint8_t LOOP_TIME_MS = 20;
        static constexpr uint16_t LOG_INTERVAL_MS = 100;

        static constexpr uint8_t BOUNDARY_ESCAPE_SPD = 100;

        static constexpr uint8_t SEARCH_SPD = 100;
        static constexpr uint8_t APPROACH_SPD = 150;

        static constexpr uint8_t ORBIT_APPROACH_SPD = 120;
        static constexpr uint8_t ORBIT_SPD = 160;
        static constexpr uint8_t ORBIT_DISTANCE = 87;
        static constexpr uint8_t ORBIT_ENTRY_TOLERANCE = 10;
        static constexpr uint8_t ORBIT_EXIT_TOLERANCE = 15;
        static constexpr uint8_t ORBIT_DEBOUNCE_MS = 100;
        unsigned long accumulatedOrbitTime = 0;

        static constexpr uint8_t CAPTURED_MAX_SPD = 250;
        static constexpr uint8_t CAPTURED_MIN_SPD = 180;
        static constexpr uint8_t ENTER_ALIGNMENT_TOLERANCE = 10;
        static constexpr uint8_t EXIT_ALIGNMENT_TOLERANCE = 25;
        static constexpr uint8_t ALIGNED_DEBOUNCE_MS = 0;
        static constexpr uint16_t SPEED_RAMP_MAX_MS = 700;
        unsigned long accumulatedAlignedTime = 0;

        PIDController approachPID = PIDController(0.5, 0, 0, 0.0, 1.0);
        PIDController orbitTangentPID = PIDController(0.04, 0, 0.001, -1.0, 1.0);
        PIDController orbitDistancePID = PIDController(0.3, 0, 0.001, -0.1, 1.0);

        static constexpr uint8_t TURN_SPD = 100;
        static constexpr uint8_t HEADING_TOLERANCE = 15;
        static constexpr uint8_t BALL_TILT_RANGE = 70;
        PIDController headingPID = PIDController(0.015, 0.001, 0.0, -1.0, 1.0);

        static constexpr uint16_t ESCAPE_DURATION = 100;
        elapsedMillis elapsedEscapeTime = ESCAPE_DURATION;
        float escapeDirection = 0.0f;

        elapsedMillis elapsedLastTime;
        State robotState = State::SEARCH;
        float targetHeading;

        bool handleEdgeDetection(const float dt);
        void handleHeadingCorrection(const float dt, const float targetHeading);
        void handleTargetHeading();
        void checkRobotState(const float dt, const float targetBallHeading);
        void maneuverAroundBall(const float dt, const float targetBallHeading);

        Button button;
        UartIRSensor irSensor;
        Drive drive;
        IMU imu;
        std::unique_ptr<OpticalOdometry> odometry;
        ColourSensor colourSensor;
        Logger logger;
};
