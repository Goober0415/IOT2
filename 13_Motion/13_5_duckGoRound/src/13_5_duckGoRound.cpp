/*
 * Project DuckGoRound
 * Author: Jamie Gavina
 * Date: 11/20/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "Stepper.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);
const signed int SPR = 2048;
const int currentTime = millis();

//signed int steps = 500;
int lastTime, deltaT;

Stepper myStepper(SPR, A1, A5, A2, S4);

void setup()
{

    myStepper.setSpeed(10);
}

void loop()
{
        
        myStepper.step(SPR*2);
        delay(500);
        myStepper.step(-SPR);
        delay(500);
    
}