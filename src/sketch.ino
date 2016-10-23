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
#if 0
    int x = 0;
    while (x == 0) {
        x = Serial.parseInt();
    }
    int y = 0;
    while (y == 0) {
        y = Serial.parseInt();
    }
    Serial.print(x);
    Serial.print(" ");
    Serial.println(y);
    arm.set(x, y);
    delay(250);
#else
    Position pos[] = {
        {90,90},
        {60,120},
        {90,90},
        {60,90},
        {10,90},
        {60,90},
        };
    int size = sizeof(pos) / sizeof(pos[0]);
    for (int i=0; i<size; i++) {
        arm.set(pos[i].angle1, pos[i].angle2);
        delay(1000);
    }
#endif
}
