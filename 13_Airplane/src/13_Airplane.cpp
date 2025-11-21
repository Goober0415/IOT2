// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"


byte accelxH, accelxL, accelyH, accelyL, accelzH, accelzL;
int16_t accelX, accelY, accelZ;
float gravityX, gravityY, gravityZ, pitch, roll;
int rotation;
const int MPUADDR = 0x68;
unsigned int currentTime, lastSecond;
String dateTime, timeOnly;

int getRotation(float x, float y);
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

const int OLED_RESET = -1;
Adafruit_SSD1306 display(OLED_RESET);

// setup() runs once, when the device is first turned on
void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();
  Time.zone(-6); // MST = -7, MDT = -6
  Particle.syncTime(); // Sync time with Particle Cloud
  //Begin I2C communications
  Wire.begin();
  //Begin transmission to MPU-6050
  Wire.beginTransmission(MPUADDR);
  //Select and write to PWR_MGMT1 register
  Wire.write(0x6B);
  Wire.write(0x00);//Wakes up MPU-6050
  //End transmission and close connection
  Wire.endTransmission(true);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  //Set the "pointer" to the 0x3B memory location of the MPU and wait for data
  Wire.beginTransmission(MPUADDR);
  Wire.write(0x3B); //Starting with the register 0x3B
  Wire.endTransmission(false);//Send the send pointer command and keep active

  dateTime = Time.timeStr();
  timeOnly = dateTime.substring(11,19);
  //Request and then read 6 bytes
  //Syntax:
  //Wire.requestFrom(I2C addr, quantity, stop);
  //Wire.read(); //Repeat for each byte

  Wire.requestFrom(MPUADDR, 6, true);
  accelxH = Wire.read(); // x accel MSB
  accelxL = Wire.read(); // x accel LSB
  accelyH = Wire.read(); // y accel H 
  accelyL = Wire.read(); // y accel L
  accelzH = Wire.read(); // Z accel H
  accelzL = Wire.read(); // z accel L

  accelX = accelxH << 8 | accelxL; //Bit shift values
  accelY = accelyH << 8 | accelyL;
  accelZ = accelzH << 8 | accelzL;
  gravityX = 1/16383.5 * accelX + 0.0; // convert to gravity
  gravityY = 1/16383.5 * accelY + 0.0;
  gravityZ = 1/16383.5 * accelZ + 0.0;
  currentTime = millis();
  if((currentTime - lastSecond)>1000){
    lastSecond = millis();
    Serial.printf("X-axis acceleration is %0.2f\n", gravityX);
    Serial.printf("Y-axis acceleration is %0.2f\n", gravityY);
    Serial.printf("Z-axis acceleration is %0.2f\n", gravityZ);
  }
  
  rotation = getRotation(gravityX, gravityY);
  pitch = -asin(gravityX);
  roll = atan2(gravityY, gravityZ);
  display.setRotation(rotation);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.printf(timeOnly);
  display.printf("\nX axis: %0.5f", gravityX);
  display.printf("\nY axis: %0.5f", gravityY);
  //display.printf("\nPitch: %f", pitch);
  float degreeP = pitch * 180/M_PI; 
  display.printf("\nPitch: %f", degreeP);
  //display.printf("\nRoll: %f", roll);
  
  float degreeR = roll * 180/M_PI;
  display.printf("\nRoll: %f", degreeR);
  display.display();
  display.clearDisplay();
}

int getRotation(float x, float y){
  if(x < 0.3 && y < .8 ){
    rotation = 0;
  }
  if(x < 0.1 && y > 0.8){
    rotation = 1;
  }
  if(x > 0.5 && y > 0.1){
    rotation = 2;
  }
  if(x > 0.7 && y < 0.1){
    rotation = 3;
  }
  return rotation;
}
