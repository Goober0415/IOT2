/* 
 * Project: L09_00_Function
 * Description: Example of using Functions
 * Author: Brian Rashap
 * Date: 15-JAN-2024
 */

#include "Particle.h"
#include "math.h"
#include "Button.h"
#include "Encoder.h"
#include "neopixel.h"
#include "Colors.h"

// Create Encoder Object
Encoder myEnc(D15,D16);
const int SWPIN = D17;
const int REDPIN = D19;
const int GREENPIN = D18;

// Create Button Object
Button encButton(SWPIN);

// Create NeoPixel Object
const int PIXEL_COUNT = 16;
Adafruit_NeoPixel pixel(PIXEL_COUNT, SPI1, WS2812B);

// Declare Constants
const float fast = 0.2;
const float slow = 10;
const int blinkPeriod = 750;

// Declare Variables
float period;
float frequency;
bool blinkOn;

// Declare Functions
float getPeriod(float minPeriod, float maxPeriod);
int bindEncoder(int value, int min, int max);
void neoSine(float freq, int color = 0x0000FF);
void pixelFill(int start, int end, int color);
void blinkLED(int LEDPIN, int blinkPeriod);
float mapFloat(float value, float inMin, float inMax, float outMin, float outMax);

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
  pixel.begin();
  pixel.show();

  pinMode(REDPIN,OUTPUT);
  myEnc.write(47);        // set encoder midway to start
}

void loop() {
  period = getPeriod(fast,slow);
  frequency = 1 / period;
  neoSine(frequency,0xFF0000);

  if(encButton.isClicked()) {
    blinkOn = !blinkOn;
  }

  if(blinkOn) {
    blinkLED(REDPIN,blinkPeriod);
  }
  else {
    digitalWrite(REDPIN,HIGH);
  }
}

// read encoder and determine desired period
float getPeriod(float minPeriod, float maxPeriod) {
  float result;
  int encValue;

  encValue = bindEncoder(myEnc.read(),0,95);
  result = mapFloat(encValue, 0, 95, minPeriod, maxPeriod);

  return result;
}

// bind Encoder between min and max
int bindEncoder(int value, int min, int max) {
  int boundValue;

  boundValue = value;
  if(value < min) {
    boundValue = min;
    myEnc.write(min);
  }
  if(value > max) {
    boundValue = max;
    myEnc.write(max);
  }
  return boundValue;
}

// light up neopixel with brightness driven by sine wave
void neoSine(float freq, int color){
  int brightness;
  float t;

  t = millis()/1000.0;
  brightness = 40*sin(2*M_PI*freq*t) + 50;
  pixelFill(4,15,(brightness/255.0)*color);
}

// will all neopixels between start and end with color
void pixelFill(int start, int end, int color) {
  int i;
  for(i=start;i<=end;i++){
    pixel.setPixelColor(i,color);
  }
  pixel.show();
}

// function to blink LED without delays
void blinkLED(int LEDPIN, int blinkPeriod) {
  static int lastTime = -99999;
  int currentTime;
  static bool LEDToggle = false;

  currentTime = millis();
  if((currentTime-lastTime) > blinkPeriod) {
    lastTime = currentTime;
    LEDToggle = !LEDToggle;
    digitalWrite(LEDPIN, LEDToggle);
  }
}

// Brian's map function that works with floats
float mapFloat(float value, float inMin, float inMax, float outMin, float outMax) {
  float newValue;

  newValue = value * ((outMax-outMin)/(inMax-inMin)) + outMin;
  return newValue;
}