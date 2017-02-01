#include <Servo.h>
#include <IRremote.h>
#include "Sg90Servo.h"

const int IRRECV_PIN   = 11; // MOSI(pin#17)
const int STEERING_PIN = 8;  // IO8 (pin#14)

const unsigned long IRRECV_LEFT    = 0x41B619E6;
const unsigned long IRRECV_RIGHT   = 0x41B66996;
const unsigned long IRRECV_UP      = 0x41B6F906;
const unsigned long IRRECV_DOWN    = 0x41B67986;
const unsigned long IRRECV_ENTER   = 0x41B659A6;
const unsigned long IRRECV_SLEEP30 = 0x41B6A15E;
const unsigned long IRRECV_SLEEP60 = 0x41B6619E;

class SteeringController
{
private:
    #define STEERING_MIN    0
    #define STEERING_INIT   90
    #define STEERING_MAX    180
    #define STEERING_OFFSET 10

    Sg90Servo servo;
    int   angle;

public:
    SteeringController() : angle(STEERING_INIT), servo(STEERING_PIN, STEERING_INIT) {
    }

    void initialize() {
        servo.initialize();
    }

    void left(int offset = STEERING_OFFSET) {
        int nextAngle = angle + offset;
        if (nextAngle > STEERING_MAX) {
            nextAngle = STEERING_MAX;
        }
        set(nextAngle);
        angle = nextAngle;
    }

    void right(int offset = STEERING_OFFSET) {
        int nextAngle = angle - offset;
        if (nextAngle < STEERING_MIN) {
            nextAngle = STEERING_MIN;
        }
        set(nextAngle);
        angle = nextAngle;
    }

private:
    void set(int angle, bool enableCache=true) {
        if (enableCache && angle == this->angle) {
            return;
        }

        servo.write(angle);
        servo.wait();
        servo.close();
    }
};

SteeringController steering;
IRrecv irrecv(IRRECV_PIN);

void dump(decode_results* results)
{
    int count = results->rawlen;
    if (results->decode_type == UNKNOWN) {
        Serial.print("Unknown encodint: ");
    } else if (results->decode_type == NEC) {
        Serial.print("Decorded NEC: ");
    } else {
        Serial.print("Unknown encodint: ");
    }
    Serial.print(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.print("bits)");
    Serial.print("Raw (");
    Serial.print(count, DEC);
    Serial.println(")");
}

void setup()
{
    Serial.begin(9600);
    irrecv.enableIRIn();
    Serial.println("Booted.");
    steering.initialize();
}

void ircontrol()
{
    decode_results results;
    if (!irrecv.decode(&results)) {
        return;
    }
    irrecv.resume();

    dump(&results);
    switch (results.value) {
    case IRRECV_LEFT:  steering.left();    break;
    case IRRECV_RIGHT: steering.right();   break;
    case IRRECV_DOWN:  steering.left(30);  break;
    case IRRECV_UP:    steering.right(30); break;
    }
}

void uartcontrol()
{
    int ch = Serial.read();
    if (ch == -1) {
        return;
    }

    Serial.write(ch);
    switch (ch) {
    case 'l': steering.left();    break;
    case 'r': steering.right();   break;
    case 'L': steering.left(30);  break;
    case 'R': steering.right(30); break;
    }
}

void loop()
{
    ircontrol();
    uartcontrol();
}
