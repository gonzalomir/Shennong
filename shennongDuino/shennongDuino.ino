#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>

//datastreams
char sensor1ID[] = "Humedad_Ambiente";
char sensor2ID[] = "Temperatura_Ambiente";
char sensor3ID[] = "Humedad_Suelo";

XivelyDatastream datastreams[] = {
  XivelyDatastream(sensor1ID, strlen(sensor1ID), DATASTREAM_FLOAT),
  XivelyDatastream(sensor2ID, strlen(sensor2ID), DATASTREAM_FLOAT),
  XivelyDatastream(sensor3ID, strlen(sensor3ID), DATASTREAM_FLOAT)
};

IPAddress ip(192,168,0,250);
EthernetClient client;
XivelyFeed feed(xivelyFeed, datastreams, 3 /* number of datastreams */);
XivelyClient xivelyclient(client);

byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x3E, 0xA0};

//IPAddress server(216,52,233,122);      // numeric IP for api.xively.com
//char server[] = "api.xively.com";   // name address for xively API

DHT dht;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting single datastream upload to Xively...");
  Serial.println();
  dht.setup(3); // data pin 2
  pinMode(2,OUTPUT); //GND
  pinMode(4,OUTPUT); //VCC
  digitalWrite(2,LOW);
  digitalWrite(4,HIGH);
  Ethernet.begin(mac, ip);
}

void loop()
{
  float humedad=dht.getHumidity();
  float temperatura=dht.getTemperature();

  int sensorValue = analogRead(0);
  int sensor=map(sensorValue,300,900,0,100);
  datastreams[0].setFloat(humedad);
  datastreams[1].setFloat(temperatura);
  datastreams[2].setFloat(sensor);

  //send value to xively
  Serial.println("Uploading it to Xively");
  int ret = xivelyclient.put(feed, xivelyKey);
  //return message
  Serial.print("xivelyclient.put returned ");
  Serial.println(ret);
  Serial.println("");

  //delay between calls
  delay(2*60000);
}




