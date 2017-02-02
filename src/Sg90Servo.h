#pragma once

#include <Servo.h>


class Sg90Servo
{
private:
    Servo servo;
    int   pin;
    int   angle;

public:
    Sg90Servo(int pin, int angle) : pin(pin), angle(angle) {
    }

    void initialize() {
        write(angle);
        wait();
        close();
    }

    void write(int angle) {
        // Incase of using IRrecv.
        //
        // Attach and detach everytime.
        // If do not, the servo vibrate slightly.
        servo.attach(pin);
        servo.write(angle);
        this->angle = angle;
    }

    void wait() {
        // Case delay(160)
        //   If do write() wait() close(), can not actuate once.
        // Case delay(170)
        //   If do write() wait() close(), can actuate everytime.
        delay(200);
    }

    void close() {
        servo.detach();
    }
};
