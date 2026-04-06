#include <Robot.hpp>

Robot robot;

void setup() {
    Serial.begin(9600);
}

void loop() {
    robot.run();
}