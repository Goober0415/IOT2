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

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);
const int MPUADDR = 0x68;
byte gyroZh, gyroZL;
int16_t gyroZ;
float angularVelocity, degree, steps;
const int DELTAT = 100;
unsigned int lastTime;
Stepper stepper(2048, D3, D4, D5, D6);
// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here
  //bigStepper.setSpeed(12);
  Wire.begin();
  //Begin transmission to MPU-6050
  Wire.beginTransmission(MPUADDR);
  //Select and write to PWR_MGMT1 register
  Wire.write(0x47);
  Wire.write(0x00);//Wakes up MPU-6050
  //End transmission and close connection
  Wire.endTransmission(true);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  if((millis()-lastTime) > DELTAT){
    lastTime = millis();
    Wire.beginTransmission(MPUADDR);
    Wire.write(0x47);
    Wire.endTransmission(false);

    Wire.requestFrom(MPUADDR, 2, true);
    gyroZh = Wire.read();
    gyroZL = Wire.read();

    gyroZ = gyroZh << 8 | gyroZL; //bit shift
    Serial.printf("Z Gyro: %i\n", gyroZ);
    angularVelocity = (1/131.0)*gyroZ; //angular velocity
    degree = w_z * (DELTAT/1000.0); // per second
    steps = (2048/360.0)*degree; // steps based on the degree read
    stepper.setSpeed(12);
    stepper.step(steps);
  }
  

}