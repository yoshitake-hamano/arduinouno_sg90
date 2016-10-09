#include <Servo.h>

class ServoController
{
private:
    Servo servo;
    int   offset;
    int   angle;
    int   max_angle;
    int   min_angle;

public:
    ServoController(int offset,
                    int angle,
                    int max_angle,
                    int min_angle) {
        this->offset    = offset;
        this->angle     = angle;
        this->max_angle = max_angle;
        this->min_angle = min_angle;
    }

    void attach(int pin) {
        servo.attach(pin);
    }

    void next() {
        angle += offset;
        if (angle > max_angle) {
            angle  = max_angle;
            offset = -offset;
        }
        if (angle < min_angle) {
            angle  = min_angle;
            offset = -offset;
        }
    }

    void set(int angle) {
        servo.write(angle);
    }

    void actuate() {
        servo.write(angle);
    }
};

#define SERVO1_OFFSET        1
#define SERVO1_INITIAL_ANGLE 90
#define SERVO1_MIN_ANGLE     80
#define SERVO1_MAX_ANGLE     90
#define SERVO2_OFFSET        1
#define SERVO2_INITIAL_ANGLE 90
#define SERVO2_MIN_ANGLE     80
#define SERVO2_MAX_ANGLE     90

ServoController servo1(SERVO1_OFFSET,
                       SERVO1_INITIAL_ANGLE,
                       SERVO1_MAX_ANGLE,
                       SERVO1_MIN_ANGLE);
ServoController servo2(SERVO2_OFFSET,
                       SERVO2_INITIAL_ANGLE,
                       SERVO2_MAX_ANGLE,
                       SERVO2_MIN_ANGLE);


void setup()
{
    Serial.begin(9600);
    servo1.attach(4);
    servo1.actuate();

    servo2.attach(5);
    servo2.actuate();

}

void loop()
{
    servo1.actuate();
    servo1.next();

    servo2.next();
    servo2.actuate();
    delay(1000);
}
