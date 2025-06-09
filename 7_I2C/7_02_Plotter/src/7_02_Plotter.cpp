/*
 * Project Plotter
 * Author: Jamie Gavina
 * Date:10/22/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ssd1306.h"
#include "Adafruit_BME280.h"
#include "neopixel.h"
#include "Colors.h"
#include "Wire.h"

SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

const int OLED_RESET = -1;
const int HEXADDRESS = 0X76;
bool status;
float temp;
int currentTime = millis();
int lastSecond = 500;
const int PIXELCOUNT = 16;
const int DEG = 0XB0;
const int PERCENT = 0X25;

Adafruit_SSD1306 display(OLED_RESET);
Adafruit_BME280 bme; // I2C
Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

void plotter(int temp, int time);

void setup()
{
    Serial.begin(9600);
    waitFor(Serial.isConnected, 1000);
    Wire.begin();
    Serial.printf("Serial monitor ready");
    status = bme.begin(HEXADDRESS);
    if (status == false)
    {
        Serial.printf("BME failed to start at %i", HEXADDRESS);
    }
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display(); // show splashscreen
    if ((currentTime - lastSecond) > 50)
    {
        display.clearDisplay(); // clears the screen and buffer
    }
}

void loop()
{
    plotter(temp, currentTime);
}

void plotter(int temperature, int time)
{
    temperature = (bme.readTemperature());
    time = currentTime;
    if ((currentTime - lastSecond) > 500)
    {
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(time, temperature);
        display.printf(".");
        display.display();
    }
}