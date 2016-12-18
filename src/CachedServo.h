#pragma once

#include <Servo.h>


class CachedServo
{
private:
    Servo servo;
    int   pin;
    int   angle;

public:
    CachedServo(int pin, int angle) : pin(pin), angle(angle) {
    }

    void initialize() {
        forceWrite(angle);
    }

    void write(int angle) {
        if (angle == this->angle) {
            return;
        }

        forceWrite(angle);
    }

private:
    void forceWrite(int angle) {
        // Incase of using IRrecv.
        //
        // Attach and detach everytime.
        // If do not, the servo vibrate slightly.
        servo.attach(pin);
        servo.write(angle);
        delay(120);
        servo.detach();
        this->angle = angle;
    }
};
