/*
 * Project microStepper
 * Author: Jamie Dowden-Duarte
 * Date: 10/28/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */
// Include Particle Device OS APIs
#include "Particle.h"
#include "Button.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int EN = D3, DIR = D4, PUL = D5, BUT = D6, BBUT = D10;
int x;
bool high, onOff;

Button black(BUT);
Button blue(BBUT);

void stepperOn();

void setup()
{
    Serial.begin(9600);
    waitFor(Serial.isConnected, 10000);
    pinMode(EN, OUTPUT);
    pinMode(DIR, OUTPUT);
    pinMode(PUL, OUTPUT);
    digitalWrite(EN, LOW); // When EN is in the valid state (offline mode)
}

void loop()
{
    for (x = 0; x < 2200; x++)
    {
        digitalWrite(DIR, HIGH);
        digitalWrite(PUL, HIGH);
        delayMicroseconds(5000);
    }
    digitalWrite(PUL, LOW);
    delayMicroseconds(5000);

    for (x = 0; x < 2200; x++)
    {
        digitalWrite(DIR, LOW);
        digitalWrite(PUL, HIGH);
        delayMicroseconds(5000);
    }
    digitalWrite(PUL, LOW);
    delayMicroseconds(5000);

    for (x = 0; x < 2200; x++)
    {
        stepperOn();
        digitalWrite(DIR, HIGH);
        digitalWrite(PUL, HIGH);
        delayMicroseconds(5000);
    }
    digitalWrite(PUL, LOW);
    delayMicroseconds(5000);

    for (x = 0; x < 8000; x++)
    {
        stepperOn();
        if (black.isClicked())
        {
            high = !high; // dir
            Serial.printf("Clicked\n");
            delayMicroseconds(1000);
        }
        if (high)
        {
            digitalWrite(DIR, HIGH);
            Serial.printf("forward\n"); // left
        }
        if (!high)
        {
            digitalWrite(DIR, LOW);
            Serial.printf("backward\n"); // right
        }
        if (blue.isClicked())
        {
            break;
            onOff = !onOff;
            if (onOff)
            {
                digitalWrite(PUL, LOW);
                delayMicroseconds(5000);
            }
            if (!onOff)
            {
                digitalWrite(PUL, HIGH);
                delayMicroseconds(5000);
            }
        }
    }
}

void stepperOn()
{

    digitalWrite(PUL, HIGH);
    delayMicroseconds(5000);
    digitalWrite(PUL, LOW);
    delayMicroseconds(5000);
}
