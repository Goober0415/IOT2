/*
 * Project DriveByWire
 * Author: Jamie Gavina
 * Date: 11/20/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

#include "Particle.h"
#include "Stepper.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

const int MPUADDR = 0x68, DELTAT = 100, IN1 = D3, IN2 = D4, IN3 = D5, IN4 = D6, ROTATION = 2048;
const float LSBSENSITIVITY = (1 / 131.0), CIRCDEGREES = 360.0, SECOND = 1000.0;
float angularVelocity, degree, steps;
unsigned int lastTime;
byte gyroZh, gyroZL;
int16_t gyroZ;

Stepper stepper(ROTATION, IN1, IN3, IN2, IN4);

void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPUADDR);
  Wire.write(0x47);
  Wire.write(0x00);
  Wire.endTransmission(true);
}

void loop()
{
  if ((millis() - lastTime) > DELTAT)
  {
    lastTime = millis();
    Wire.beginTransmission(MPUADDR);
    Wire.write(0x47);
    Wire.endTransmission(false);

    Wire.requestFrom(MPUADDR, 2, true);
    gyroZh = Wire.read();
    gyroZL = Wire.read();

    gyroZ = gyroZh << 8 | gyroZL;
    Serial.printf("Z Gyro: %i\n", gyroZ);
    angularVelocity = LSBSENSITIVITY * gyroZ;     // gyroZ/131
    degree = angularVelocity * (DELTAT / SECOND); // angleVelocity * change in time/second
    steps = (ROTATION / CIRCDEGREES) * degree;
    stepper.setSpeed(12);
    stepper.step(steps);
  }
}