/*
 * Project DriveByWire
 * Author: Jamie Gavina
 * Date: 11/20/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "Stepper.h"

SYSTEM_MODE(AUTOMATIC);
const signed int SPR = 2048;             // Steps per revolution
const float SLOPE = 0.0076, DELTAT = 50; // Delay time in milliseconds
const int HEXADDRESS = 0x68;             // MPU6050 I2C address
unsigned currentTime = millis();
int lastTime, steps;
byte gyro_z_l, gyro_z_h;

float angVZ, deltaAngle;

Stepper myStepper(SPR, A1, A5, A2, S4);

void setup()
{
  Wire.begin();
  Wire.beginTransmission(HEXADDRESS);
  Wire.write(0x6B); // Power management register
  Wire.write(0x00); // Reset the sensor
  Wire.endTransmission(true);

  Wire.beginTransmission(HEXADDRESS);
  Wire.write(0x47); // Gyroscope configuration register
  Wire.write(0x00); // Set full scale range to ±250 °/s
  Wire.endTransmission(true);

  myStepper.setSpeed(10);
}

void loop()
{
  if ((currentTime - lastTime) < DELTAT)
  {
    // Read gyroscope data
    Wire.beginTransmission(HEXADDRESS);
    Wire.write(0x47); // Start at register
    Wire.endTransmission(false);
    Wire.requestFrom(HEXADDRESS, 6, true);
    gyro_z_h = Wire.read();
    gyro_z_l = Wire.read();
    float gyro_z = (gyro_z_h << 8 | gyro_z_l); // Convert to °/s

    Serial.printf("angvz: %.2f\n", gyro_z);

    angVZ = gyro_z / 131.0f;
    Serial.printf("angvz**: %.2f\n", angVZ);

    deltaAngle = angVZ * DELTAT / 1000.0f;
    Serial.printf("deltaAngle: %.2f\n", deltaAngle);
    steps = deltaAngle * (2048 / 360.0f); // 0.17? or 5.68

    if (deltaAngle > 25 || deltaAngle < -25)
    {
      myStepper.step(steps);
      Serial.printf("steps = %i\n", steps);
      delay(100);
    }
  }
  lastTime = currentTime;
}
