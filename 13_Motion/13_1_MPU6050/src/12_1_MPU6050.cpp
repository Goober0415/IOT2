/*
 * Project MPU6050
 * Author: Jamie Gavina
 * Date: 11/18/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

SYSTEM_MODE(AUTOMATIC);

const float ACCEL_SCALE_FACTOR = 16384.0; // Default for ±2g range
const float GRAVITY = 9.80665;            // Standard gravity in m/s^2

byte accel_x_h, accel_x_l, accel_y_h, accel_y_l, accel_z_h, accel_z_l;
float accel_x, accel_y, accel_z;

void setup()
{
  Wire.begin();

  // Initialize MPU6050
  Wire.beginTransmission(0x68);
  Wire.write(0x6B); // Power management register
  Wire.write(0x00); // Wake up device
  Wire.endTransmission(true);

  // Set accelerometer range to ±2g
  Wire.beginTransmission(0x68);
  Wire.write(0x1C); // Accelerometer configuration register
  Wire.write(0x00); // ±2g range
  Wire.endTransmission(true);
}

void loop()
{
  Wire.beginTransmission(0x68);
  Wire.write(0x3B); // Start reading from ACCEL_XOUT_H
  Wire.endTransmission(false);

  Wire.requestFrom(0x68, 6, true);

  if (Wire.available() == 6)
  {
    accel_x_h = Wire.read();
    accel_x_l = Wire.read();
    accel_y_h = Wire.read();
    accel_y_l = Wire.read();
    accel_z_h = Wire.read();
    accel_z_l = Wire.read();

    accel_x = ((accel_x_h << 8 | accel_x_l) / ACCEL_SCALE_FACTOR) * GRAVITY;
    accel_y = ((accel_y_h << 8 | accel_y_l) / ACCEL_SCALE_FACTOR) * GRAVITY;
    accel_z = ((accel_z_h << 8 | accel_z_l) / ACCEL_SCALE_FACTOR) * GRAVITY;

    Serial.printf("X-axis acceleration: %.2f g\n", accel_x / GRAVITY);
    Serial.printf("Y-axis acceleration: %.2f g\n", accel_y / GRAVITY);
    Serial.printf("Z-axis acceleration: %.2f g\n", accel_z / GRAVITY);
  }
  else
  {
    Serial.println("Error reading from MPU6050");
  }

  delay(1000); // Adjust as needed
}