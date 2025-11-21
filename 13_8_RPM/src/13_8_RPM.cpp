/*
 * Project RPM
 * Author: Jamie Gavina
 * Date: 11/21/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"

SYSTEM_MODE(AUTOMATIC);

const int HALL_PIN = D5, OLEDRESET = -1, ADDY = 0X3C;
unsigned int pulses = 0, currentTime = millis(), lastTime;
static unsigned int lastMagnetPass = 0, interval = 60000; // 60 seconds

void checkRPM();
int returnRPM(pulses);

Adafruit_SSD1306 display(OLEDRESET);

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, ADDY);
  pinMode(HALL_PIN, INPUT_PULLUP);
  attachInterupt(HALL_PIN, checkRPM, RISING);
}

void loop()
{
  if ((lastTime - currentTime) > interval)
  {
    Serial.printf(returnRPM(pulses));
    pulses = 0;
  }
}

int returnRPM(pulses)
{
  int RPM = pulses / 60000.0;
  return RPM;
}

void checkRPM()
{
  pulses++;
}
