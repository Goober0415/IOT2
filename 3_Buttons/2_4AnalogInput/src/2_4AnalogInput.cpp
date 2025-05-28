/* 
 * Project l03_04_AnalogInput
 * Author: Jamie Gavina
 * Date: 1/29/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int POTENTIAMETER = A2;
int val = 0;

void setup() {
Serial.begin(9600);
waitFor(Serial.isConnected,10000);
Serial.printf("Ready to go\n");
}

void loop() {
 val = analogRead(POTENTIAMETER);
 Serial.printf("%i\n", val);
 delay(10);
}