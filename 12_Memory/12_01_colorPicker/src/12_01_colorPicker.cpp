/*
 * Project ColorPicker
 * Author: Jamie Gavina
 * Date: 11/07/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

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