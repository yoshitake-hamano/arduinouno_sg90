#include <Servo.h>
#include <IRremote.h>

const int IRSEND_PIN   = 13; // SCK(pin#19)
const int RECV_PIN     = 6;

IRsend irsend;
IRrecv irrecv(RECV_PIN);

const int REGZA_SIZE = 32;

enum {
    REGZA_ENTER,
    REGZA_UP,
    REGZA_DOWN,
    REGZA_LEFT,
    REGZA_RIGHT,
    REGZA_BLUE,
    REGZA_RED,
    REGZA_GREEN,
    REGZA_YELLOW,
};

const uint32_t REGZA_LED_DATA[] = {
    0x02FDBC43,
    0x02FD7C83,
    0x02FDFC03,
    0x02FDFA05,
    0x02FDDA25,
    0x02FDCE31,
    0x02FD2ED1,
    0x02FDAE51,
    0x02FD6E91,
};


void setup()
{
    Serial.begin(9600);
    Serial.println("Starting...");
    irrecv.enableIRIn();
}

void loop()
{
#if 0
    decode_results results;
    if (irrecv.decode(&results)) {
        if (results.decode_type == NEC) {
            Serial.print("NEC: ");
        } else if (results.decode_type == SONY) {
            Serial.print("SONY: ");
        } else if (results.decode_type == RC5) {
            Serial.print("RC5: ");
        } else if (results.decode_type == RC6) {
            Serial.print("RC6: ");
        } else if (results.decode_type == UNKNOWN) {
            Serial.print("UNKNOWN: ");
        }
        Serial.println(results.value, HEX);
        irrecv.resume(); // Receive the next value
    }
#endif

    Serial.println("Send");
    irsend.sendNEC(REGZA_LED_DATA[REGZA_ENTER], REGZA_SIZE);
    delay(random(1000, 10000));
}
