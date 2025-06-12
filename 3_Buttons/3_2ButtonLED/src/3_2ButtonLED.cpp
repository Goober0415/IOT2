/* 
 * Project l03_02_buttonLED
 * Author: Jamie Gavina
 * Date: 1/28/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int LED = D6;
const int BUTTON = D3;
bool isPressed, onOff;

void setup() {
Serial.begin(9600);
waitFor(Serial.isConnected, 10000);
Serial.printf("Ready to go\n");
pinMode(BUTTON, INPUT_PULLDOWN);
pinMode(LED,OUTPUT);
}

void loop() {
isPressed = digitalRead(BUTTON);
if(isPressed == true){
 onOff != onOff; 
 Serial.printf("Button pressed\n");
}
if (onOff == true){
 digitalWrite(LED, HIGH);
}else{
  Serial.printf(".\n");
  digitalWrite(LED, LOW);
}
}