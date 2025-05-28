/* 
 * Project HelloLED
 * Author: Jamie Gavina
 * Date: 1/28/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

int i;

void setup() {
  pinMode(D1, OUTPUT);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  i = i + 100;
  digitalWrite(D1, HIGH);
  delay(1000);
  digitalWrite(D1,LOW);
  delay(1000 + i);
}