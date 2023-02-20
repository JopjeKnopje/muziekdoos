#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

// #define ATTINY

#define PIN_BTN 2

#define RX 3
#define TX 4

#define VOLUME 20

SoftwareSerial softwareSerial(RX, TX);
DFRobotDFPlayerMini player;

unsigned long lastDebounceTime = 0;

void trigger_high(void)
{
    player.randomAll();
}

void trigger_low(void)
{
    player.stop();
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
            {
#ifndef ATTINY
                Serial.println("high");
#endif
                trigger_high();
            }
            if (oldButtonState == LOW)
            {

#ifndef ATTINY
                Serial.println("low");
#endif
                trigger_low();
            }
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
    pinMode(PIN_BTN, INPUT_PULLUP);
    randomSeed(analogRead(5));

    if (!player.begin(softwareSerial))
    {
#ifndef ATTINY
        Serial.println("unable to begin mp3 player...");
#endif

        while (true);
    }
#ifndef ATTINY
    Serial.println("player online");
#endif

    softwareSerial.setTimeout(500);
    player.volume(VOLUME);
    player.enableLoopAll();
}

void loop(void)
{
    check_button(PIN_BTN);
}