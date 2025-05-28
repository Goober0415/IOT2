/* 
 * Project helloLEDsin
 * Author: Jamie Gavina
 * Date: 1/28/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include <math.h>

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int LED = D1;
float y;

void setup() {
  pinMode(LED,OUTPUT);
}

void loop() {
y = 127.5 *sin(2 * M_PI * 1 / 5.0 * millis() / 1000.0) + 127.5;
analogWrite(LED, y);
}
