#include <Arduino.h>

#include <drive/Drive.hpp>
#include <odometry/Odometry.hpp>
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
        static constexpr uint8_t LOOP_TIME_MS = 10;

        static constexpr uint8_t SEARCH_SPD = 60;
        static constexpr uint8_t APPROACH_SPD = 100;
        static constexpr uint8_t ORBIT_APPROACH_SPD = 100;
        static constexpr uint8_t ORBIT_SPD = 100;
        static constexpr uint8_t CAPTURED_SPD = 100;
        static constexpr uint8_t BOUNDARY_BACK_SPD = 100;

        static constexpr uint8_t APPROACH_DISTANCE_TOLERANCE = 100;
        static constexpr uint8_t ENTER_ALIGNMENT_TOLERANCE = 10;
        static constexpr uint8_t EXIT_ALIGNMENT_TOLERANCE = 18;
        static constexpr uint8_t ORBIT_DISTANCE = 100;

        static constexpr uint8_t ALIGNED_DEBOUNCE_MS = 50;
        unsigned long accumulatedAlignedTime = 0;

        PIDController approachPID = PIDController(1, 0, 0, -APPROACH_SPD, APPROACH_SPD);
        PIDController orbitTangentPID = PIDController(1, 0, 0, -ORBIT_SPD, ORBIT_SPD);
        PIDController orbitDistancePID = PIDController(1, 0, 0, -ORBIT_APPROACH_SPD, ORBIT_APPROACH_SPD);

        elapsedMillis elapsedLastTime;
        State robotState = State::SEARCH;

        bool handleEdgeDetection(const float dt);
        void maneuverAroundBall(const float dt, const float targetBallHeading);
        void checkRobotState(const float dt, const float targetBallHeading);

        Button button;
        QikEasy irSensor;
        Drive drive;
        IMU imu;
        OpticalOdometry odometry;
        ColourSensor colourSensor;
};
