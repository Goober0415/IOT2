/* 
 * Project millisFunction
 * Author: Jamie Dowden-Duarte
 * Date: 12/2/25
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

//As an unsigned int, millis() will circle back to 0 after hitting it's largest value 
//which takes roughly 52 days, which will undoubtedly throw off your code if your using timers. 
//Brian wrote a function that will stuff millis() in a "larger box" 
//and keep millis() going essentially indefinitely. Feel free to implement this into your code.

SYSTEM_MODE(AUTOMATIC);

uint64_t millis64bit();


void setup() {

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
 
}

uint64_t millis64bit() {
    static uint32_t low4bytes, high4bytes;
    uint32_t newMillis;

    newMillis = millis();
}