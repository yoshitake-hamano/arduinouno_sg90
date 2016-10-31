#include <Servo.h>
#include <IRremote.h>

class SteeringController
{
private:
    Servo servo;

public:
    void attach(int pin) {
        servo.attach(pin);
    }

    void set(int angle) {
        servo.write(angle);
    }
};

class Position
{
public:
    int angle;

    Position(int angle) {
        this->angle = angle;
    }
};

SteeringController steering;

void setup()
{
    Serial.begin(9600);
    steering.attach(4);
}

void loop()
{
#if 1
    int x = 0;
    while (x == 0) {
        x = Serial.parseInt();
    }
    Serial.println(x);
    steering.set(x);
    delay(250);
#else
    Position pos[] = {
        {70},
        {75},
        {80}, // center
        {85},
        {90},
        {85},
        {80}, // center
        {75},
        };
    int size = sizeof(pos) / sizeof(pos[0]);
    for (int i=0; i<size; i++) {
        steering.set(pos[i].angle);
        delay(1000);
    }
#endif
}
