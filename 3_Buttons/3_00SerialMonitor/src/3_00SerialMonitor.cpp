/* 
 * Project l03_00_SerialMonitor
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
  Serial.begin(9600);
  waitFor (Serial.isConnected, 10000);
  Serial.println ("Ready to go");

}

void loop() {
Serial.println("Hello World");

for(i = 0; 1 <= 13; i++){
  Serial.print(i + ",\n");
}
for(i = 0; 1 <= 13; i++){
  Serial.println(i + ",\n");
}
for(i = 0; 1 <= 13; i++){
  Serial.printf("%i, \n", i);
}
}
