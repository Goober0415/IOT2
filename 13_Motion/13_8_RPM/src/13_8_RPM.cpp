/*
 * Project RPM
 * Author: Jamie Gavina
 * Date: 11/21/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
SYSTEM_MODE(AUTOMATIC);

const int HALL_PIN = D5;
volatile unsigned long pulses = 0;
static unsigned long lastMagnetPass = 0;
static unsigned int interval = 60000; // 60 seconds

void checkRPM();
void onSensorChange();

Timer rpmTimer(interval, checkRPM, false);

void setup()
{
  pinMode(HALL_PIN, INPUT_PULLUP);
  attachInterrupt(HALL_PIN, onSensorChange, RISING);
  Serial.begin(9600);
  rpmTimer.start();
}

void loop()
{
  // This function is empty as all the work is done in the timer interrupt
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
  if (millis() - lastMagnetPass <= interval)
  {
    float rpm = 60000 / lastMagnetPass;
    Serial.print("RPM: ");
    Serial.println(rpm);
    pulses = 0;
    lastMagnetPass = millis();
  }
}
