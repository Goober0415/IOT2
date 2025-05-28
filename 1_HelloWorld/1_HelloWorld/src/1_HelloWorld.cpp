/* 
 * Project 1_HelloWorld
 * Author: Jamie Gavina 
 * Date:1/28/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC)

void setup() {
  pinMode(D7, OUTPUT);
}

void loop() {
 digitalWrite(D7,HIGH);
 delay(16);
 digitalWrite(D7,LOW);
 delay(16);
}