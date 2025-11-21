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
void onSensorChange();

Adafruit_SSD1306 display(OLEDRESET);
Timer rpmTimer(interval, checkRPM);

void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, ADDY);
  pinMode(HALL_PIN, INPUT_PULLUP);
  rpmTimer.start();
  attachInterrupt(HALL_PIN, onSensorChange, CHANGE);
}

void loop()
{
  if ((currentTime - lastTime) > interval)
    checkRPM();
  lastTime = currentTime;
}

void onSensorChange()
{
  unsigned long currentTime = millis();
  if (currentTime > lastMagnetPass)
  {
    pulses++;
    lastMagnetPass = currentTime;
  }
}

void checkRPM()
{
  float rpm = 60000 / lastMagnetPass;
  display.printf(rpm);
  pulses = 0;
  lastMagnetPass = millis();
}
