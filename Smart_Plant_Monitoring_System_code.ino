/*
Code Name:Smart Plant Monitoring System
Author: Avishek MAndal
Description: This program is used to Designed an IoT-based plant monitoring system using ESP8266,
 Blynk,  and sensors for temperature, humidity, and water level. Enabled real-time monitoring and
 alerts through a mobile app for optimized plant care. Focused on efficient sensor integration and 
 user-friendly IoT connectivity.
*/

#define BLYNK_TEMPLATE_ID "TMPL3fXVB5bAs"
#define BLYNK_TEMPLATE_NAME "smart"
#define BLYNK_AUTH_TOKEN "xfb5t5rsqxbhpuXc5clCy6-Yw8p_hLq5"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "xfb5t5rsqxbhpuXc5clCy6-Yw8p_hLq5"; 
char ssid[] = "RANA";  
char pass[] = "Rana@1234";  

#define soil A0    
#define RELAY_PIN_1       D0 
#define PUSH_BUTTON_1     D7  
#define VPIN_BUTTON_1    V12 

DHT dht(D4, DHT11);
BlynkTimer timer;

int relay1State = LOW;
double T, P;
char status;
void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);
  digitalWrite(RELAY_PIN_1, relay1State);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();
  timer.setInterval(100L, soilMoistureSensor);
  timer.setInterval(100L, DHT11sensor);
  
}

void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
}
void soilMoistureSensor() {
  int value = analogRead(soil);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;
  Blynk.virtualWrite(V3, value);
}

BLYNK_CONNECTED() {
   Blynk.syncVirtual(VPIN_BUTTON_1);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  relay1State = param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
}
void loop() {
  Blynk.run();
  timer.run();
  }
