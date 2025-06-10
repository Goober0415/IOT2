/*
 * Project L09_00_HelloReset
 * Description: Reset IoT Classroom Credentials
 * Author: Brian Rashap
 * Date: 11-MAR-2023
 */
#include "Particle.h"

void getWiFi();

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected,10000);
  delay(1000);

  WiFi.clearCredentials();
  Serial.printf("Has Credentials = %i\n\n",WiFi.hasCredentials());

  delay(1000);

  WiFi.setCredentials("DDCIOT","ddcIOT2020");

  // Connect to Internet but not Particle Cloud
  WiFi.on();
  WiFi.connect();
  while(WiFi.connecting()) {
    Serial.printf(".");
    delay(100);
  }
  Serial.printf("\n\n");
  delay(3000);

  // Next Connect to Particle Cloud
  while(!Particle.connected()) {
    Particle.connect();
    delay(100);           //Small delay needed
    Serial.printf("x");
  }
  Serial.printf("\n\n");
  delay(3000);

  getWiFi();

  pinMode(D7,OUTPUT);
}

void loop() {
  if(WiFi.hasCredentials()) {
    digitalWrite(D7,HIGH);
    delay(250);
    digitalWrite(D7,LOW);
    delay(250);
  }
}


void getWiFi() {
  WiFiAccessPoint ap[10];
  int sectype,cytype,found,i;
  byte mac[6];

  String security[6] = {"WLAN_SEC_UNSEC", "WLAN_SEC_WEP", "WLAN_SEC_WPA", "WLAN_SEC_WPA2", "WLAN_SEC_WPA_ENTERPRISE", "WLAN_SEC_WPA2_ENTERPRISE"};
  String cypher[4] = {"N/A","WLAN_CIPHER_AES", "WLAN_CIPHER_TKIP", "WLAN_CIPHER_AES_TKIP"};

  Serial.printf("Scan Argon for WiFi Information \n");
  Serial.printf("ip address: %s \n", WiFi.localIP().toString().c_str());
  WiFi.macAddress(mac);
  Serial.printf("mac: %02X:%02X:%02X:%02X:%02X:%02X \n", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
  found = WiFi.getCredentials(ap, 5);
  Serial.printf("Credentials for %d Access Points Found \n",found);
  for (i = 0; i < found; i++) {
    sectype = (int) ap[i].security;
    cytype = (int) ap[i].cipher;
    for(int j=0;j<6;j++){
       mac[j] = ap[i].bssid[j];
    }
      
    Serial.printf("AP%d - ssid: %s \n", i,ap[i].ssid);
    Serial.printf("AP%d - mac: %02X:%02X:%02X:%02X:%02X:%02X \n", i,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    Serial.printf("AP%d - security: %d (security %s) \n", i,sectype,security[sectype].c_str());
    Serial.printf("AP%d - cipher:   %d (cipher %s) \n\n", i,cytype, cypher[cytype].c_str());
  }
}