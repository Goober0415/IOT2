/*
 * Project BME280
 * Author: Jamie Dowden-Duarte
 * Date: 10/22/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "neopixel.h"
#include "Colors.h"
#include "Adafruit_BME280.h"
#include "Button.h"
#include "IoTTimer.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

const int OLED_RESET = -1 , HEXADDRESS = 0X76, SEALEVELPRESSURE_HPA = 1013.25, DEG = 0XB0 , PERCENT = 0X25,PIXELCOUNT = 16 , UPDATE_INTERVAL = 5000;
bool status;
float tempF, inHG,temp, pressure, humidity;

Adafruit_SSD1306 display(OLED_RESET);
Adafruit_BME280 bme; // I2C
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
IoTTimer timer;

void printSensorData();
void updateDisplay(float temp, float pressure, float humidity);
void updateNeoPixels(float temo, float pressure, float humidity);


void setup()
{
    Serial.begin(9600);
    waitFor(Serial.isConnected, 1000);

    Wire.begin();
    Serial.printf("Ready to go\n");

    status = bme.begin(HEXADDRESS);

    if (status == false)
    {
        Serial.printf("BME at address %i failed to start\n", HEXADDRESS);
    }

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    display.display(); // show splashscreen
    
    timer.startTimer(500);
    pixel.begin();
    pixel.setBrightness(10);
    pixel.show();
}

void loop()
{
  float temp = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F; // Convert Pa to hPa
  float humidity = bme.readHumidity();
tempF = (temp * 9 / 5.0) + 32;
inHG = (humidity * 0.00029530);

if (timer.isTimerReady()){

    printSensorData();
    updateDisplay(tempF, inHG, humidity);
    updateNeoPixels(tempF, inHG, humidity);
  }
}

void printSensorData()
{
  Serial.printf("Temperature: %0.1f%c\n Humidity: %0.1f\n Pressure %0.1f", tempF,DEG,inHG);
}

void updateDisplay(float temp, float pressure, float humidity)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(0, 0);
    display.printf("Temperature: %0.1f%c\n Humidity: %0.1f\n Pressure %0.1f", tempF,DEG,inHG);
    display.display();
}

void updateNeoPixels(float temp, float pressure, float humidity)
{
    pixel.clear();

    if (temp >= 80.00)
    {
        pixel.setBrightness(10);
        pixel.setPixelColor(PIXELCOUNT, red);
    }
    else if (temp < 79.00 && temp > 40.00)
    {
        pixel.setBrightness(15);
        pixel.setPixelColor(PIXELCOUNT, green);
    }
    else
    {
        pixel.setBrightness(5);
        pixel.setPixelColor(PIXELCOUNT, blue);
    }

    if (pressure >= 50.00)
    {
        pixel.setPixelColor(PIXELCOUNT - 1, violet);
    }
    else
    {
        pixel.setPixelColor(PIXELCOUNT - 1, maize);
    }

    if (humidity < 49.00)
    {
        pixel.setPixelColor(PIXELCOUNT - 2, orange);
    }
    else
    {
        pixel.setPixelColor(PIXELCOUNT - 2, yellow);
    }
    pixel.show();
  }
