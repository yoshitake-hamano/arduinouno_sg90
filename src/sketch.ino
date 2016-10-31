#include <Servo.h>
#include <IRremote.h>

const int IRRECV_PIN   = 11;
const int MOTOR_PIN    = 9;
const int STEERING_PIN = 4;

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
    const int STEERING_MIN    = 66;
    const int STEERING_INIT   = 78;
    const int STEERING_MAX    = 90;
    const int STEERING_OFFSET = 4;

    Servo servo;
    int   index;

public:
    void attach(int pin) {
        index = 0;
        servo.attach(pin);
        servo.write(STEERING_INIT);
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
};

class MotorController
{
private:
    int pin;

public:
    void attach(int pin) {
        this->pin = pin;
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    void up() {
        digitalWrite(pin, HIGH);
    }

    void down() {
        digitalWrite(pin, LOW);
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
    steering.attach(STEERING_PIN);
    irrecv.enableIRIn();
    motor.attach(MOTOR_PIN);
}

void loop()
{
    decode_results results;
    if (!irrecv.decode(&results)) {
        return;
    }

    dump(&results);
    irrecv.resume();
    switch (results.value) {
    case IRRECV_LEFT:  steering.left();  break;
    case IRRECV_RIGHT: steering.right(); break;
    case IRRECV_UP:    motor.up();       break;
    case IRRECV_DOWN:  motor.down();     break;
    }
    delay(50);
}
