#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>


#define ATTINY

#define PIN_BTN 2
#define PIN_LED 1

#define RX 3
#define TX 4

#define VOLUME 20

SoftwareSerial softwareSerial(RX, TX);
DFRobotDFPlayerMini player;

unsigned long lastDebounceTime = 0;

void trigger(void)
{
    static int ledState = 0;
    player.randomAll();
    digitalWrite(PIN_LED, ledState);
    ledState = !ledState;
}

bool check_button(uint8_t pin)
{
    static int oldButtonState = HIGH;
    static int lastButtonState = HIGH;

    int currentButtonState = digitalRead(pin);

    if (currentButtonState != lastButtonState)
        lastDebounceTime = millis();

    if ((millis() - lastDebounceTime) > 50)
    {
        if (currentButtonState != oldButtonState)
        {
            oldButtonState = currentButtonState;
            if (oldButtonState == HIGH)
                return true;
        }
    }
    lastButtonState = currentButtonState;
    return false;
}

void setup(void)
{
#ifndef ATTINY
    Serial.begin(9600);
#endif
    delay(2000);
    softwareSerial.begin(9600);

    pinMode(PIN_LED, OUTPUT);

    pinMode(PIN_BTN, INPUT_PULLUP);
    randomSeed(analogRead(5));

    digitalWrite(PIN_LED, LOW);

    if (!player.begin(softwareSerial))
    {
#ifndef ATTINY
        Serial.println("unable to begin mp3 player...");
#endif

        digitalWrite(PIN_LED, HIGH);
        while (true);
    }
#ifndef ATTINY
    Serial.println("player online");
#endif

    softwareSerial.setTimeout(500);
    player.volume(VOLUME);
    player.enableLoopAll();
    digitalWrite(PIN_LED, LOW);
}

void loop(void)
{
    bool val = check_button(PIN_BTN);
    if (val)
    {
#ifndef ATTINY
        Serial.println("yup");
#endif
        trigger();
    }
}