const int SOLENOID_PIN = 9;  // IO9 (pin#15)

void setup()
{
    Serial.begin(9600);
    Serial.println("Booted.");
    pinMode(SOLENOID_PIN, OUTPUT);
}

static void push()
{
    Serial.println("push 1");
    digitalWrite(SOLENOID_PIN, HIGH);
    delay(200);
    digitalWrite(SOLENOID_PIN, LOW);
}

void loop()
{
    int ch = Serial.read();
    if (ch == -1) {
        return;
    }

    Serial.write(ch);
    switch (ch) {
    case '1': push(); break;
    }
}
