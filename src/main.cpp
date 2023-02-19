#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

#define PIN_BTN 2

#define RX 5
#define TX 4

#define VOLUME 20

SoftwareSerial softwareSerial(RX, TX);
DFRobotDFPlayerMini player;

unsigned long lastDebounceTime = 0;

void trigger(void)
{
    player.randomAll();
}

bool check_button(uint8_t pin)
{
    static int oldButtonState;
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
    Serial.begin(9600);
    softwareSerial.begin(9600);

    pinMode(PIN_BTN, INPUT_PULLUP);
    randomSeed(analogRead(5));

    if (!player.begin(softwareSerial))
    {
        Serial.println("unable to begin mp3 player...");
        while (true);
    }
    Serial.println("player online");

    softwareSerial.setTimeout(500);
    player.volume(VOLUME);
    player.enableLoopAll();
}

void loop(void)
{
    bool val = check_button(PIN_BTN);
    if (val)
    {
        Serial.println("yup");
    }
}
