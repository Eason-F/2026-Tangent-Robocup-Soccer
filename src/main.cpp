#include <Robot.hpp>

Robot robot;

void setup() {
    Serial.begin(115200);
}

void loop() {
    robot.run();
}
