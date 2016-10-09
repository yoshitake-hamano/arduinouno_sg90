#include <Servo.h>

class RobotArmController
{
private:
    Servo servo1;
    Servo servo2;

public:
    void attach(int pin1, int pin2) {
        servo1.attach(pin1);
        servo2.attach(pin2);
    }

    void set(int angle1, int angle2) {
        servo1.write(angle1);
        servo2.write(angle2);
    }
};

class Position
{
public:
    int angle1;
    int angle2;

    Position(int angle1, int angle2) {
        this->angle1 = angle1;
        this->angle2 = angle2;
    }
};

RobotArmController arm;

void setup()
{
    Serial.begin(9600);
    arm.attach(4, 5);
}

void loop()
{
    Position pos[] = {
        {90,48},
        {90,47},
        {90,46},
        {90,45},
        {90,44},
        {90,43},
        {90,42},
        {90,41},
        {90,40},
        {90,41},
        {90,42},
        {90,43},
        {90,44},
        {90,45},
        {90,46},
        {90,47},
        {90,48},
        {90,49},
        {89,49},
        {88,48},
        {87,47},
        {85,47},
        {83,47},
        {81,47},
        {80,47},
        {81,47},
        {83,47},
        {85,47},
        {87,47},
        {88,48},
        {89,49},
    };
    int size = sizeof(pos) / sizeof(pos[0]);
    for (int i=0; i<size; i++) {
        arm.set(pos[i].angle1, pos[i].angle2);
        delay(250);
    }
}
