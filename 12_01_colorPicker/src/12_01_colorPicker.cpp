/*
 * Project ColorPicker
 * Author: Jamie Gavina
 * Date: 11/07/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

<<<<<<< HEAD
#include "Particle.h"
#include "neopixel.h"
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "credentials.h"

const int PIXELCOUNT = 2;
int color;
byte r, g, b,red,green,blue;
byte buf[6];
int len = EEPROM.length();
int addr = 0x00AE;
int val = 0x45;

void MQTT_connect();
bool MQTT_ping();
void pixelFill(int startP, int endP, int hex);
void colorChange(int color, byte *r, byte *g, byte *b);

/************ Global State (you don't need to change this!) ***   ***************/ 
TCPClient TheClient; 

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 

Adafruit_MQTT_Subscribe colors = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/colors");

Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

void setup() {
  Serial.begin(9600);
  pixel.begin();
  pixel.setBrightness(80);
  pixel.setPixelColor(1,56,57,58);
  pixel.setPixelColor(0,54,55,56);
  pixel.show();
  EEPROM.write(addr, r);
  EEPROM.write(addr+1, g);
  EEPROM.write(addr+2, b);
  red = EEPROM.read(addr);
  green = EEPROM.read(addr+1);
  blue = EEPROM.read(addr+2);

  mqtt.subscribe(&colors);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  MQTT_connect();
  MQTT_ping();
 pixel.clear();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(100))) {
    if(subscription == &colors){
      Serial.printf("Received from Adafruit: %s \n",(char *)colors.lastread );
      memcpy (buf, &colors.lastread[1],6); // strip off the ’#’

      Serial.printf("Buffer: %s \n",(char *)buf);
      color = strtol((char *) buf, NULL,16); // convert string to int (hex)
      Serial.printf("Buffer: 0x%02X \n", color);
      colorChange(color, &r, &g, &b);

     
      Serial.printf("R: 0x%02X | G: 0x%02X | B: 0x%02X", r, g, b);
      
      Serial.printf("\nRed: 0x%02X | Green: 0x%02X | Blue: 0x%02X", red, green, blue);

      for(int i = 0; i < PIXELCOUNT; i++){
      pixel.setPixelColor(i,red,green,blue);
      pixel.show();
      }
    }
  }
}

void colorChange(int color, byte *r, byte *g, byte *b){
  pixel.clear();
  *r = (color>>16)&0xFF;
  *g = (color>>8)&0xFF;
  *b = (color)&0xFF;
}
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;
 
  // Return if already connected.
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.printf("Error Code %s\n",mqtt.connectErrorString(ret));
       Serial.printf("Retrying MQTT connection in 5 seconds...\n");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds and try again
  }
  Serial.printf("MQTT Connected!\n");
}

bool MQTT_ping() {
  static unsigned int last;
  bool pingStatus;

  if ((millis()-last)>120000) {
      Serial.printf("Pinging MQTT \n");
      pingStatus = mqtt.ping();
      if(!pingStatus) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      last = millis();
  }
  return pingStatus;
}

void pixelFill(int startP, int endP, int hex){
  for(int i = startP; i <= endP; i++){
    pixel.setPixelColor(i, hex);
    pixel.show();
  }

    int8_t ret;
    
    if (mqtt.connected()) {
        return;
    }
    
    Serial.print("Connecting to MQTT... ");
    while ((ret = mqtt.connect()) != 0) {
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000);
    }
    Serial.println("MQTT Connected!");
}
=======
 #include "Particle.h"
 #include <Adafruit_MQTT.h>
 #include "Adafruit_MQTT_SPARK.h"
 #include "credentials.h"
 #include "math.h"
 #include "neopixel.h"

TCPClient TheClient;

byte r, g, b,buf[6],data[] = {0x23 ,0x42 ,0x41 ,0x34 ,0x32 ,0x35 ,0x44 ,0x39 ,0x35 };
int colorValue, lastmsglen, lastread;
String payload;
int color;
bool pingStatus;
unsigned long lastPing;
const int PIXELCOUNT = 16;

  SYSTEM_MODE(SEMI_AUTOMATIC);

 Adafruit_MQTT_SPARK mqtt(&TheClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 Adafruit_MQTT_Subscribe color_sub = Adafruit_MQTT_Subscribe(&mqtt, "your-feed-id/colors");
 Adafruit_NeoPixel pixel(PIXELCOUNT, SPI1, WS2812B);

 void MQTT_connect();
 bool MQTT_ping();
 void convertHex(int color, byte *r, byte *g, byte *b);
 
 void setup()
 {
   Serial.begin(9600);
   waitFor(Serial.isConnected, 10000);
 
   WiFi.on();
   WiFi.connect();
   while (WiFi.connecting())
   {
     Serial.printf(".");
   }
   Serial.println("\n\nConnected to Wi-Fi");
 
   MQTT_connect();
   mqtt.subscribe(&color_sub);
   pixel.begin();
   pixel.setBrightness(10);
 }
 
 void loop()
 {
   MQTT_connect();
   MQTT_ping();
 
   Adafruit_MQTT_Subscribe *subscription;
   while(subscription = mqtt.readSubscription(1000))
   {
     if (subscription == &color_sub){
       Serial.printf("recieved from adafruit: %s\n",(char *)color_sub.lastread);
       memcpy ( buf , &color_sub.lastread[1],6);
       Serial.printf("Buffer:%s\n",(char *)buf);
       color = strtol ((char *)buf,NULL,16); // convert string to int (hex)
       Serial.printf("Buffer:0x%02X\n",color);
      // Convert hex to RGB components
      convertHex(colorValue, &r,&g,&b);
      pixel.setPixelColor(PIXELCOUNT, color);
      pixel.show();
     }
   }
 delay(500);
 }
 
 void convertHex(int color, byte *r, byte *g, byte *b)
 {
  int i = 0XFF;
  *r = (color >> 16) & i;
  *g = (color >> 8) & i;
  *b = color & i;
 }
 
 bool MQTT_ping()
 {
   static unsigned long lastPing = 0;
   bool pingStatus;
 
   if ((millis() - lastPing) > 120000)
   {
     Serial.println("Pinging MQTT...");
     pingStatus = mqtt.ping();
     if (!pingStatus)
     {
       Serial.println("Disconnecting...");
       mqtt.disconnect();
     }
     lastPing = millis();
   }
   return pingStatus;
 }
 
 void MQTT_connect()
 {
   int8_t ret;
 
   if (mqtt.connected())
   {
     return;
   }
 
   Serial.print("Connecting to MQTT... ");
   while ((ret = mqtt.connect()) != 0)
   {
     Serial.println(mqtt.connectErrorString(ret));
     Serial.println("Retrying MQTT connection in 5 seconds...");
     mqtt.disconnect();
     delay(5000);
   }
   Serial.println("MQTT Connected!");
 }
>>>>>>> 8e6d6496df3c4e7620db7992f89d3cd9b0f75e14
