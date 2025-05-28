/* 
 * Project l03_03_twoButtonLED
 * Author: Jamie Gavina
 * Date: 1/28/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

//pins
const int REDLED = D6;
const int YELLOWLED = D5;
const int YELLOW =D2;
const int GREY = D3;

bool greyPressed;
bool yellowPressed;

void setup() {
 Serial.begin(9600);
 waitFor(Serial.isConnected, 100000);
 Serial.printf("Ready to go\n");
 pinMode(YELLOW, INPUT_PULLDOWN);
 pinMode(GREY,INPUT_PULLDOWN);
 pinMode(REDLED, OUTPUT);
 pinMode(YELLOWLED, OUTPUT);
}


void loop() {
greyPressed = digitalRead(GREY);
yellowPressed = digitalRead(YELLOW);

if (greyPressed == true) {
    digitalWrite(REDLED,HIGH);
    Serial.printf("Grey pressed\n");
}else{
    digitalWrite(REDLED,LOW);
    Serial.printf(".\n");
}

if (yellowPressed == true) {
    digitalWrite(YELLOWLED,HIGH);
    Serial.printf("Yellow pressed\n");
}else{
    digitalWrite(YELLOWLED,LOW);
    Serial.printf(".\n");
}
}