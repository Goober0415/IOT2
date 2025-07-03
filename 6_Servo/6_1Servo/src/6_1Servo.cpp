/* 
 * Project l06_01_Servo
 * Author: Jamie Gavina
 * Date: 02/07/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "math.h"
#include "Button.h"

//global variables
const int PWM = D16, BUTTON = D1;

float sine;
bool Switch;

//objects
Button Btn(BUTTON);
Servo myServo;


SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
  myServo.attach(PWM);
}

void loop() {
  sine = 90*sin(2*M_PI*(1/5.0)*millis()/1000.0)+90;

if (Btn.isClicked()){
   click = !click;
}
if (click == true){
   myServo.write(sine);
}
}