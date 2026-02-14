#include <Arduino.h>

class Button {
    private:
        int buttonPin = 4;
        bool buttonState;
    public:
        Button() {
            pinMode(buttonPin, INPUT_PULLUP);
        }

        bool isPressed() {
            return digitalRead(buttonPin);
        }
};