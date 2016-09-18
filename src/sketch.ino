#include <Servo.h>

#define ANGLE_OFFSET 1

Servo servo;

void setup()
{
    Serial.begin(9600);
    servo.attach(4);
}

void loop()
{
    static int angle;
    static int offset = ANGLE_OFFSET;
    angle += offset;
    if (angle > 180) {
        angle  = 180;
        offset = -offset;
    }
    if (angle < 0) {
        angle  = 0;
        offset = -offset;
    }

    Serial.print("servo : ");
    Serial.println(angle);
    servo.write(angle);
}
