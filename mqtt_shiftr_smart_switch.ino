
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ArduinoJson.h> 
#ifndef LED_BUILTIN
#define LED_BUILTIN
#endif
#define RELEPIN 14 // change accordingly your device hardware, this is for Wemos D1 R1

 char ssid[] = "VAIO_Network"; //  your wifi SSID (name)
 char pass[] = "imda-z76m-ylz9"; // your wifi password 
 int keyIndex = 0;            
 int status = WL_IDLE_STATUS;
 WiFiClient net;
 MQTTClient client;
 unsigned long lastMillis = 0;
 // MQTT info
 const char* thehostname = "broker.shiftr.io";
 const char* user = "changeToyourUser";
 const char* user_password = "changeToyourPassword";
 const char* id = "ESP8266-SmartSwitch";
 boolean OnOff = false;

 void connect() {
   Serial.print("checking wifi…");
   while (WiFi.status() != WL_CONNECTED) {
     Serial.print(".");
     delay(1000);
   }
 Serial.print("\nconnecting…");
   while (!client.connect(id, user, user_password)) {
     Serial.print(".");
     delay(1000);
   }
 Serial.println("\nconnected!");
   client.subscribe("/14oy-client"); //change the value 14oy to your device id, check the value in Cloud Firestore database
 }
 
 void messageReceived(String &topic, String &payload) {
   Serial.println("incoming: " + topic + " - " + payload);
   DynamicJsonBuffer jsonBuffer;
   JsonObject& json = jsonBuffer.parseObject(payload);   
   String deviceOn = json ["on"];     

   if (deviceOn == "true"){
     digitalWrite(LED_BUILTIN, LOW);
     digitalWrite(RELEPIN, HIGH);
     OnOff = true;
   }
   if (deviceOn == "false"){
     digitalWrite(LED_BUILTIN, HIGH);
     digitalWrite(RELEPIN, LOW);
     OnOff = false;
   }

 }
 
 void setup() {
   //Initialize serial and wait for port to open:
   Serial.begin(115200);
   //Setup Rest Client
   WiFi.begin(ssid, pass);

   pinMode(LED_BUILTIN, OUTPUT);
   pinMode(RELEPIN, OUTPUT);

   client.begin(thehostname, net);
   client.onMessage(messageReceived);
   connect();
   delay(1000);
 }
 int timer =0;
 void loop() {
   client.loop();
   if (!client.connected()) {
     connect();
   }
 }
