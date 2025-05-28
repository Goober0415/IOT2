/* 
 * Project l03_04_AnalogInputRevisited
 * Author: Jamie Gavina
 * Date: 1/29/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int POTENTIOMETER = A2;
int val;
float volt;

float voltConversion(int val);

void setup() {
Serial.begin(9600);
waitFor(Serial.isConnected, 100000);
Serial.printf("Ready to go\n");
}

void loop() {
  val = analogRead(POTENTIOMETER);
  Serial.printf("raw value : %i\n", val);
  delay(10);
  voltConversion(val);
  Serial.printf("%.1f\n", volt);
  delay(5000);
}

float voltConversion(int val){
volt = (3.3/4095.0)* val;
return volt;
}