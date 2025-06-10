/*
 * Project Hue
 * Author: Jamie Gavina
 * Date: 10/24/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "hue.h"
#include "Button.h"
#include "wemo.h"
#include "Encoder.h"
#include "Colors.h"

SYSTEM_MODE(MANUAL);

Button BUTTON(D16);
Button BLACKBUTTON(D15);
const int ENCODERA = D8;
const int ENCODERB = D9;
Encoder ENCODER(ENCODERA, ENCODERB);
const int BULB = 4;
int color;
bool buttonState;
int lastPosition = 0;
unsigned int lastUpdateTime = 0;
unsigned int currentTime = millis();
unsigned int lastSecond = 5000;
const int currentPosition = ENCODER.read();
int position;
int brightness;

void setup()
{
    Serial.begin(9600);
    waitFor(Serial.isConnected, 15000);

    WiFi.on();
    WiFi.clearCredentials();
    WiFi.setCredentials("IoTNetwork");
    WiFi.connect();
    while (WiFi.connecting())
    {
        Serial.printf(".");
    }
    Serial.printf("\n\n");
}

void loop()
{
    if (BUTTON.isClicked())
    {
        Serial.printf("on/off\n");
        buttonState = !buttonState;
        Serial.printf("Setting color of bulb %i to color %06i\n", BULB, HueRainbow[color % 7]);
        setHue(BULB, true, HueRainbow[color % 7], random(32, 255), 255);
    }
    position = ENCODER.read();

    if (position != lastPosition)
    {
        brightness = position;
        Serial.printf("Brightness of bulb %i\n", brightness);
        setHue(BULB, buttonState, rainbow[color % 7], brightness, 255);
    }
    if (BLACKBUTTON.isClicked())
    {
        color++;
        setHue(BULB, buttonState, rainbow[color % 7], brightness, 255);
    }
}
