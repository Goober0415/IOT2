/*
 * Project myScale
 * Author: Jamie Gavina
 * Date: 11/08/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "HX711.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

HX711 myScale(D6, D5);

const int CALFACTOR = 562;
const int SAMPLES = 10;
float weight, rawData, calibration;
int offset;

void setup()
{
  Serial.begin(9600);
  waitFor(Serial.isConnected, 1000);
  myScale.set_scale();
  delay(5000);
  myScale.tare();
  myScale.set_scale(CALFACTOR);
}

void loop()
{
  weight = myScale.get_units(SAMPLES);
  rawData = myScale.get_value(SAMPLES);
  delay(5000);
  calibration = myScale.get_scale();
  offset = myScale.get_offset();
  delay(5000);
  Serial.printf("weight: %f\n", weight*-1);
  Serial.printf("rawdata: %f\n", rawData);
  Serial.printf("calibration: %f\n",calibration);
  Serial.printf("offset: %i\n", offset);
}