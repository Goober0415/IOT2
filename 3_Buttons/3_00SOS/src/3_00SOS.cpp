/* 
 * Project Review l03_00_SOS
 * Author: Jamie Gavina
 * Date: 1/29/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
SYSTEM_MODE(SEMI_AUTOMATIC);

const int LED = SDA;
const int BUTTON = D7;

bool isPressed;

void SOS(int segment);
void flasher(int duration);

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(BUTTON, INPUT_PULLDOWN);
    digitalWrite(LED,LOW);
}

void loop() {
    isPressed = digitalRead(BUTTON);
    
    if (isPressed == TRUE) {
        SOS(3); // Call SOS with total number of dots and dashes
    } else {
        digitalWrite(LED, LOW);
    }
}

void SOS(int segment) {
    if (segment > 0) {
        flasher(250); // Flash for 250ms (dot)
        delay(450); // Wait for 450ms between dots/dashes
        
        if (segment > 1) {
            flasher(1150); // Flash for 1150ms (dash)
            delay(450); // Wait for 450ms between dots/dashes
            
            if (segment > 2) {
                SOS(segment - 2); // Recursive call for remaining segments
            }
        }
    }
}

void flasher(int duration) {
    digitalWrite(LED, HIGH);
    delay(duration);
    digitalWrite(LED, LOW);
}