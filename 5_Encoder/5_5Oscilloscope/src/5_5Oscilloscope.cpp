/* 
 * Project l05_05_oscilloscope
 * Author: Jamie Gavina
 * Date: 2/7/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int LED = D13;
const int LEDDELAY = 5000;
int i;

void setup() {
  pinMode(LED, OUTPUT);
}


void loop() {
analogWrite(LED, 255);
delay(LEDDELAY);
analogWrite(LED, 63);
delay(LEDDELAY);
analogWrite(LED, 171);
delay(LEDDELAY);
analogWrite(LED, 16);
delay(LEDDELAY);
}
