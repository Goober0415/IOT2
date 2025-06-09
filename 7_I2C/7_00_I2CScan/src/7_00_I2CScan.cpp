/*
 * Project I2 C scan
 * Author: Jamie Gavina
 * Date: 10/21/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"

byte status, address;
int nDevices;
const unsigned int DELAYTIME = 5000;

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup()
{
  Wire.begin();                      // wake up bus
  delay(2000);
  Serial.begin(9600);                // wake up monito
  waitFor(Serial.isConnected, 1000); // wait for monitor to wake up
}

void loop()
{
  nDevices = 0;
  for (int address = 0; address < 128; address++) // loop through all the addresses
  {
    Wire.beginTransmission(address); // okay who is at this address?
    status = Wire.endTransmission(); // sends data back to tell you the answer
    if (status == 0)
    {
      Serial.printf("I2C device found at address 0x%02x\n", address);
      nDevices++;
    }
  }
  Serial.printf("%i I2C Devices found\n", nDevices);
  delay(DELAYTIME);
}