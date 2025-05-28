/* 
 * Project l03_06_HelloNightLight
 * Author: Jamie Gavina
 * Date: 1/29/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int PHOTODIODE = A0;
const int LED = D1;
String bright, LEDB;
bool isDark;
float val,bright;
int y;

void setup() {
Serial.begin(9600);
waitFor(Serial.isConnected, 10000);
Serial.printf("Ready to go");
pinMode(LED, OUTPUT);
pinMode(PHOTODIODE, INPUT);
}


void loop() {
val = analogRead(PHOTODIODE);
Serial.printf("%.1f\n", val);
delay(500);
bright = String(val);

if (val <= 2000){ 
    analogWrite(LED,63);
    LEDB = 63;
    Particle.publish("Brightness", bright, PRIVATE);
    Particle.publish("LED", LEDB, PRIVATE);
}
if (val <= 2500){
    analogWrite(LED,171);
    LEDB = 171;
    Particle.publish("Brightness", bright, PRIVATE);
    Particle.publish("LED", LEDB, PRIVATE);
}
if (val <= 3000){
    analogWrite(LED,255);
    LEDB = 255;
    Particle.publish("Brightness", bright, PRIVATE);
    Particle.publish("LED", LEDB, PRIVATE);
}
else{
    digitalWrite(LED,LOW);
}
}
