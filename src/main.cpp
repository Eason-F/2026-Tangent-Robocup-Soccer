#include <Robot.hpp>

Robot robot;

void setup() {
    Serial.begin(115200);
    robot.setup();
}

void loop() {
    robot.run();
}
