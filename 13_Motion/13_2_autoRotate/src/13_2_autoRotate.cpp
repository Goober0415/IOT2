/*
 * Project autorotate
 * Author: Jamie Gavina
 * Date: 11/18/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Wire.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX_RK.h"

SYSTEM_MODE(AUTOMATIC);

const float ACCEL_SCALE_FACTOR = 16384.0; // Default for ±2g range
const float GRAVITY = 9.80665;            // Standard gravity in m/s^2
const int OLED_RESET = -1;

Adafruit_SSD1306 display(OLED_RESET);

byte accel_x_h, accel_x_l, accel_y_h, accel_y_l, accel_z_h, accel_z_l;
float accel_x, accel_y, accel_z;
int getRotation(int x, int y, int z);

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
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
  Time.now();

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

    int rotation = getRotation(accel_x, accel_y, accel_z);

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(Time.format("%A , %B %d"));
    display.setCursor(0, 20);
    display.print(Time.format("%I : %M : %S %p"));
    display.setRotation(rotation);
    display.display();
    delay(1000);

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

int getRotation(int x, int y, int z)
{
  // Map MPU6050 orientation to OLED rotation
  if (abs(x) > abs(y) && abs(x) > abs(z))
  {           // Device is flat, facing up or down
    return 0; // No rotation
  }
  else if (abs(y) > abs(x) && abs(y) > abs(z))
  {           // Device is standing upright
    return 2; // Rotate 180 degrees
  }
  else
  { // Device is on its side
    if (z > 0)
    {           // Screen facing left
      return 1; // Rotate 90 degrees clockwise
    }
    else
    {           // Screen facing right
      return 3; // Rotate 90 degrees counter-clockwise
    }
  }
}