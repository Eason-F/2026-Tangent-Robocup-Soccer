// #include <Arduino.h>

// // main.cpp (或 sketch.ino)
// const int interruptPin = 20;
// volatile bool edgeDetected = false;

// void onFallingEdge() {
//     edgeDetected = true;
// }

// void setup() {
//     Serial.begin(9600);
//     pinMode(interruptPin, INPUT_PULLUP);
//     attachInterrupt(digitalPinToInterrupt(interruptPin), onFallingEdge, FALLING);
// }

// void loop() {
//     if (edgeDetected) {
//         edgeDetected = false;
//         Serial.println("Edge detected!");
//     }
// }