/* 
 * Project l05_06_lowPass
 * Author: Jamie Gavina
 * Date: 2/7/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "math.h"

const int LEDPIN = D13;
float t;

float brightness;
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

void setup()
{
  pinMode(LEDPIN, OUTPUT);

  t = 0;
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  brightness = 127 * sin(2 * M_PI * 1 / 2.0 * millis() / 1000.0) + 127;

  analogWrite(LEDPIN, brightness, 50000);
}