/*
 * Project L14_01_BlueTooth
 * Description: Starter Code for sending / recieving data from Bluefruit Connect app
 * Author: Brian Rashap
 * Edited: Jamie Dowden-Duarte
 * Date: 28-JUL-20212
 */

#include "Particle.h"
#include "neopixel.h"
#include "Colors.h"
#include "Encoder.h"

SYSTEM_MODE(SEMI_AUTOMATIC);
SYSTEM_THREAD(ENABLED);

// These UUIDs were defined by Nordic Semiconductor and are now the defacto standard for
// UART-like services over BLE. Many apps support the UUIDs now, like the Adafruit Bluefruit app.
const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
const size_t UART_TX_BUF_SIZE = 16;
const int PIXELCOUNT = 16;

uint8_t txBuf[UART_TX_BUF_SIZE];
uint8_t i;

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context);
int randomColor();
void pixelFill(int startPixel, int endPixel, int color);

Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

int lights, lightPosition, rColor, gColor, bColor;
int currColor = red;

// setting up the encoder
int previous, position, bluEnc, encoderPos;
const int PINA = D8;
const int PINB = D9;
Encoder myEnc(PINA, PINB);

BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);
BleAdvertisingData data;

// set up ble to send the right info
const int BUFSIZE = 95;
byte buf[BUFSIZE];

void setup()
{
  Serial.begin(9600);
  waitFor(Serial.isConnected, 15000);
  pixel.begin();
  pixel.setBrightness(10);
  pixel.show();
  BLE.on();
  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);
  data.appendServiceUUID(serviceUuid);
  BLE.advertise(&data);
  Serial.printf("Photon2 BLE Address: %s\n", BLE.address().toString().c_str());
}

void loop()
{

  pixel.show();
  position = myEnc.read();
  if (position != previous)
  {

    rColor = random(0, 255);
    gColor = random(0, 255);
    bColor = random(0, 255);
    if (myEnc.read() > 95)
    {
      position = 95;
      pixel.clear();
    }
    if (myEnc.read() < 0)
    {
      position = 0;
      pixel.clear();
    }
    previous = position;
    lightPosition = previous * 16 / 95.0;
    pixel.setPixelColor(lightPosition, rColor, gColor, bColor);

    // plotter
    sprintf((char *)buf, "%i, %i, %i, %i", lightPosition, rColor, gColor, bColor);
    buf[BUFSIZE - 1] = 0x0A;

    //  Serial.printf("%s",(char *)buf);
    txCharacteristic.setValue(buf, BUFSIZE);
  }
}

int randomColor()
{
  currColor = rColor + gColor + bColor;
  return currColor;
}

// onDataReceived is used to receive data from Bluefruit Connect App
void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context)
{
  uint8_t i;
  int color = red;
  pixel.setBrightness(10);
  bluEnc = atoi((char *)data);
  myEnc.write(encoderPos);
  encoderPos = bluEnc * 6;
  pixel.clear();
  for (i = 0; i < bluEnc; i++)
  {
    pixel.setPixelColor(i, rColor, gColor, bColor);
  }
  pixel.show();
  Serial.printf("Received data from: %02X:%02X:%02X:%02X:%02X:%02X \n", peer.address()[0], peer.address()[1], peer.address()[2], peer.address()[3], peer.address()[4], peer.address()[5]);
  Serial.printf("Bytes: ");
  for (i = 0; i < len; i++)
  {
    Serial.printf("%02X", data[i]);
  }
  if (currColor != color)
  {
    if (data[0] == 0x21 && data[1] == 0x43)
    {
      color = (data[2] << 16) | (data[3] << 8) | data[4];
      pixelFill(0, 16, color);
      Serial.printf("Color: %i\n", color);
    }
  }
  Serial.printf("\n");
  Serial.printf("pixel position = %i\n", bluEnc);
  Serial.printf("encoder position = %i\n", encoderPos);
  Serial.printf("Color Picker %i\n", currColor);
}

void pixelFill(int startPixel, int endPixel, int color)
{
  int litPixel;
  for (litPixel = startPixel; litPixel <= endPixel; litPixel++)
  {
    pixel.setPixelColor(litPixel, color);
  }
  pixel.show();
}