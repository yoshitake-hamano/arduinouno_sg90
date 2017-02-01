#include <Servo.h>
#include <IRremote.h>
#include "Sg90Servo.h"
#include "Point2.h"
#include "Point3.h"
#include "ActuatorController.h"

const int IRRECV_PIN    = 11; // MOSI(pin#17)
const int MOTOR_PIN     = 9;  // IO9 (pin#15)
const int STEERING_PIN  = 4;  // IO4 (pin#6)

const int LOWER_ARM_PIN = 8;  // IO8  (pin#14)
const int UPPER_ARM_PIN = 6;  // IO6  (pin#12)
const int SWING_PIN     = 7;  // IO7  (pin#13)

const unsigned long IRRECV_LEFT    = 0x41B619E6;
const unsigned long IRRECV_RIGHT   = 0x41B66996;
const unsigned long IRRECV_UP      = 0x41B6F906;
const unsigned long IRRECV_DOWN    = 0x41B67986;
const unsigned long IRRECV_ENTER   = 0x41B659A6;
const unsigned long IRRECV_SLEEP30 = 0x41B6A15E;
const unsigned long IRRECV_SLEEP60 = 0x41B6619E;

const int LOWER_ARM_INIT_ANGLE = 90;
const int UPPER_ARM_INIT_ANGLE = 30;
const int SWING_INIT_ANGLE     = 90;
const int SWING_STEP           = 30;

double yRing[] = {102.5, 160.5};
int    yIndex  = 0;
const int Y_RING_MAX = sizeof(yRing) / sizeof(yRing[0]);
double zRing[] = { 99.5, 14.5,  -70.5, 14.5};
int    zIndex  = 0;
const int Z_RING_MAX = sizeof(zRing) / sizeof(zRing[0]);

Sg90Servo lowerArmServo(LOWER_ARM_PIN,
                        LOWER_ARM_INIT_ANGLE);
Sg90Servo upperArmServo(UPPER_ARM_PIN,
                        UPPER_ARM_INIT_ANGLE);
Sg90Servo swingServo(SWING_PIN,
                     SWING_INIT_ANGLE);

Point3 theArmRobotEndPoint(0,
                           LENGTH_OF_UPPER_ARM,
                           LENGTH_OF_LOWER_ARM);
double swingAngle = SWING_INIT_ANGLE;


class SteeringController
{
private:
    const int STEERING_MIN    = 54;
    const int STEERING_INIT   = 78;
    const int STEERING_MAX    = 102;
    const int STEERING_OFFSET = 12;

    Sg90Servo servo;
    int       index;

public:
    SteeringController()
    : servo(STEERING_PIN, STEERING_INIT) {
        index = 0;
    }

    void initialize() {
        servo.initialize();
    }

    void right() {
        int nextIndex = index + 1;
        int nextAngle = nextIndex * STEERING_OFFSET + STEERING_INIT;
        if (nextAngle > STEERING_MAX) {
            return;
        }
        index = nextIndex;
        servo.write(nextAngle);
    }

    void left() {
        int nextIndex = index - 1;
        int nextAngle = nextIndex * STEERING_OFFSET + STEERING_INIT;
        if (nextAngle < STEERING_MIN) {
            return;
        }
        index = nextIndex;
        servo.write(nextAngle);
    }

    void wait() {
        servo.wait();
    }

    void close() {
        servo.close();
    }
};

class MotorController
{
private:
    int  pin;
    bool moving;

public:
    void attach(int pin) {
        this->pin    = pin;
        this->moving = false;
        pinMode(pin, OUTPUT);
        down();
    }

    void up() {
        digitalWrite(pin, HIGH);
        moving = true;
    }

    void down() {
        digitalWrite(pin, LOW);
        moving = false;
    }

    void toggle() {
        if (moving) {
            down();
        } else {
            up();
        }
    }
};

SteeringController steering;
MotorController motor;
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
    Serial.println("Starting...");
    steering.initialize();
    irrecv.enableIRIn();
    motor.attach(MOTOR_PIN);

    lowerArmServo.initialize();
    upperArmServo.initialize();
    swingServo.initialize();
}

void loop()
{
    Point3 endpoint = theArmRobotEndPoint;

    decode_results results;
    if (!irrecv.decode(&results)) {
        return;
    }
#if 0
    dump(&results);
#endif
    irrecv.resume();

    switch (results.value) {
    case IRRECV_UP:      ++yIndex;                 break;
    case IRRECV_DOWN:    ++zIndex;                 break;
    case IRRECV_SLEEP30: swingAngle += SWING_STEP; break;
    case IRRECV_SLEEP60: swingAngle -= SWING_STEP; break;
    case IRRECV_LEFT:    steering.left();          break;
    case IRRECV_RIGHT:   steering.right();         break;
    case IRRECV_ENTER:   motor.toggle();           break;
    default: return;
    }

    if (Y_RING_MAX <= yIndex) yIndex = 0;
    if (yIndex < 0)           yIndex = Y_RING_MAX - 1;
    endpoint.y = yRing[yIndex];

    if (Z_RING_MAX <= zIndex) zIndex = 0;
    if (zIndex < 0)           zIndex = Z_RING_MAX - 1;
    endpoint.z = zRing[zIndex];

    if (swingAngle < 0)   swingAngle = 0;
    if (180 < swingAngle) swingAngle = 180;
    Point3 joint;
    double lowerArmAngle;
    double upperArmAngle;
    double dummy;
    if (!ActuatorController::getRobotArmAngle(endpoint,
                                              &joint,
                                              &lowerArmAngle,
                                              &upperArmAngle,
                                              &dummy)) {
        steering.wait();
        steering.close();
        Serial.println("Can not reach this point.");
        return;
    }

    lowerArmAngle = lowerArmAngle * 180 / M_PI;
    upperArmAngle = UPPER_ARM_INIT_ANGLE - upperArmAngle * 180 / M_PI;

    Serial.print("("); Serial.print(endpoint.x, DEC);
    Serial.print(","); Serial.print(endpoint.y, DEC);
    Serial.print(","); Serial.print(endpoint.z, DEC);
    Serial.print(")");
    Serial.print(" "); Serial.print(lowerArmAngle, DEC);
    Serial.print(","); Serial.print(upperArmAngle, DEC);
    Serial.print(","); Serial.print(swingAngle, DEC);
    Serial.println("");
    if (lowerArmAngle < 0 || 180 < lowerArmAngle) { return; }
    if (upperArmAngle < 0 || 180 < upperArmAngle) { return; }
    if (swingAngle    < 0 || 180 < swingAngle)    { return; }

    lowerArmServo.write(lowerArmAngle);
    upperArmServo.write(upperArmAngle);
    swingServo.write(swingAngle);
    theArmRobotEndPoint = endpoint;

    swingServo.wait();
    steering.close();
    lowerArmServo.close();
    upperArmServo.close();
    swingServo.close();
}
