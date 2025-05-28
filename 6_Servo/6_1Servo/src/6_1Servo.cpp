/* 
 * Project l06_01_Servo
 * Author: Jamie Gavina
 * Date: 02/07/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

Servo myServo;

void setup() {
  myServo.attach(A2); //PWM
}

void loop() {
  myServo.write(180);
  
  
}