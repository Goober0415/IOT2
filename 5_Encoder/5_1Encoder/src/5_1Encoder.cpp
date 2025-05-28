/* 
 * Project l05_01_encoder
 * Author: Jamie Gavina
 * Date: 2/6/25
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */
//NO MORE DELAYS!!!!
#include "Particle.h"
#include <Encoder.h>

const int PINA = D8;
const int PINB = D9;

unsigned long currentTime;
unsigned long lastUpdateTime;

int position;
int lastPosition;

Encoder myEnc(PINA, PINB);

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
  Serial.begin();
  waitFor(Serial.isConnected, 10000);
  Serial.printf("Ready to go\n");

}

void loop()
{
  currentTime = millis();

 int currentPosition = myEnc.read();

  if (currentPosition != lastPosition)
  {

    Serial.printf("Encoder position: %i\n", currentPosition);

    lastPosition = currentPosition;
  }

  lastUpdateTime = currentTime;

}