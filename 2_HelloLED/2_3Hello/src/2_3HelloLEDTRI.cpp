/* 
 * Project helloLEDtri
 * Author: Jamie Gavina
 * Date: 1/28/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int LED = D1;
const int LEDDELAY = 10;
int i;

void setup() {
  pinMode(LED,OUTPUT);
}

void loop() {
  for(i = 0; i < 256; i++){
    analogWrite(LED,i);
    delay(LEDDELAY);
  }
  for(i = 255; i >= 0; i--){
    analogWrite(LED,i);
    delay(LEDDELAY);
  }
}
