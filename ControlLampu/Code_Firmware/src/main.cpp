#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h>

WiFiClient net;
MQTTClient client;

const char ssid[] = "TUF";
const char pass[] = "ridho111";

const int PIN_RED = 25;
const int PIN_GREEN = 26;
const int PIN_BLUE = 27;
const int PIN_RELAY1 = 18;
const int PIN_RELAY2 = 19;



void rgb(bool red, bool green, bool blue){
  digitalWrite(PIN_RED, red);
  digitalWrite(PIN_GREEN, green);
  digitalWrite(PIN_BLUE, blue);
}

void connect(){
  rgb(1,0,0);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }
  rgb(0,1,0);

  while(!client.connect("parkir")){
    delay(500);
  }
  rgb(0,0,1);
  client.subscribe("kelasiot/gedung/parkir/#", 1);
}


void subscribe(String &topic, String &data){
  if(topic == "kelasiot/gedung/parkir/jalur1"){
    digitalWrite(PIN_RELAY1, data.toInt());
  }
  if(topic == "kelasiot/gedung/parkir/jalur2"){
    digitalWrite(PIN_RELAY2, data.toInt());
  }
}

void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_RELAY1, OUTPUT);
  pinMode(PIN_RELAY2, OUTPUT);
  
  WiFi.begin(ssid, pass);
  client.begin("broker.emqx.io", net);
  client.onMessage(subscribe);

  connect();
}

void loop() {
  client.loop();
  delay(10);
}
