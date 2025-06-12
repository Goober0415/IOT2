/*
 * Project L14_01_BlueTooth
 * Description: Starter Code for sending / recieving data from Bluefruit Connect app
 * Author: Brian Rashap
 * Date: 28-JUL-20212
 */

// Setup BLE UART

#include "Particle.h"

const size_t UART_TX_BUF_SIZE = 20;
uint8_t txBuf[UART_TX_BUF_SIZE];
uint8_t i;

// These UUIDs were defined by Nordic Semiconductor and are now the defacto standard for
// UART-like services over BLE. Many apps support the UUIDs now, like the Adafruit Bluefruit app.
const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);
BleAdvertisingData data;

SYSTEM_MODE(SEMI_AUTOMATIC); //Using BLE and not Wifi

void setup() { 
    Serial.begin();
    waitFor(Serial.isConnected, 15000);  

    BLE.on();
    BLE.addCharacteristic(txCharacteristic);
    BLE.addCharacteristic(rxCharacteristic);
    data.appendServiceUUID(serviceUuid);
    BLE.advertise(&data);

    Serial.printf("Photon2 BLE Address: %s\n", BLE.address().toString().c_str());
}

void loop() {

}

//onDataReceived is used to receive data from Bluefruit Connect App
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
  uint8_t i;

  Serial.printf("Received data from: %02X:%02X:%02X:%02X:%02X:%02X \n", peer.address()[0], peer.address()[1],peer.address()[2], peer.address()[3], peer.address()[4], peer.address()[5]);
}