/* 
 * Project l03_05_Thermistor
 * Author: Jamie Gavina
 * Date: 1/29/2025
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

const int THERMPIN = A2;
const int RsRESISTOR = 1000;
const float RMTEMPVOLT = 1650;
const float ICETEMPVOLT = 330;
const float BOILINGTVOLT = 2970;
float temp;
float volt;

float convertVoltToTemp(int volt);

void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected, 10000);
  Serial.printf("Ready to go\n");
}


void loop() {
  volt = analogRead(THERMPIN) * (3.3 / 1024.0);
  temp = convertVoltToTemp(volt);
  Serial.print("Temperature: ");
  Serial.println(temp, 2);
  delay(1000);
}

float convertVoltToTemp(float volt)
{
  if (volt <= ICETEMPVOLT)
  {
    temp = 273.15 + (log(volt / RMTEMPVOLT) / log(ICETEMPVOLT / RMTEMPVOLT)) * (-20); // Assuming ice water temperature is around -20°C
    return temp;
  }
  else if (volt >= BOILINGTVOLT)
  {temp = 373.15 + (log(volt / RMTEMPVOLT) / log(BOILINGTVOLT / RMTEMPVOLT)) * (80); // Assuming boiling water temperature is around 80°C
    return temp;
  }
  else
  {
    temp = 293.15 + (log(volt / RMTEMPVOLT) / log(RMTEMPVOLT / ICETEMPVOLT)) * (60); // Room temperature assumed to be around 20°C
    return temp;
  }
}
