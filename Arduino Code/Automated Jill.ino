                                                                  //static const uint8_t D0   = 16; //static const uint8_t D1   = 5; //static const uint8_t D2   = 4; //static const uint8_t D3   = 0; //static const uint8_t D4   = 2; //static const uint8_t D5   = 14; //static const uint8_t D6   = 12; //static const uint8_t D7   = 13; //static const uint8_t D8   = 15; //static const uint8_t D9   = 3; //static const uint8_t D10  = 1; #include <SPI.h>
#include <DHT.h>
#include "TinyGPS++.h"
#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>

#define DHTPIN D4 //GN - G //D - 3V //V - D4
#define DHTTYPE DHT11
#define BLYNK_PRINT Serial

//const int buzzer = 15;
const int gasPin = A0;
static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;
char auth[] = "";                 					//Enter your Write API key from ThingSpeak  1007d8c6fc5d4d56bcc4d0b3f4ea1041
String apiKey = "";                               	//Thingspeak
char ssid[] = "";                                   //SSID
char pass[] = "";                                   //Password
const char* server = "api.thingspeak.com";          //Server thingspeak

TinyGPSPlus gps;
SimpleTimer timer;
WiFiClient client;
WidgetMap myMap(V1);
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial ss(RXPin, TXPin);                                           //static const int RXPin = 4, TXPin = 5; //Yg dicoding begitu, kalau yg dicolok dibalik rx tx nya ---> GPIO 5=D1(connect Rx of GPS) and GPIO 4=D2(Connect Tx of GPS)


void sendSensor()
{
  if (client.connect(server, 80))                                 //"184.106.153.149" or api.thingspeak.com
  {
     
//    tone(buzzer, 1000);     //send 1kHz signal
//    delay(1000);        //for 1 sec
//    noTone(buzzer);         //stop sound
//    delay(1000);        //for 1sec
  
    Blynk.virtualWrite(V5, dht.readHumidity());
    Blynk.virtualWrite(V6, dht.readTemperature());                //Or dht.readTemperature(true) for Fahrenheit
    Blynk.virtualWrite(V7, analogRead(gasPin));                   //const int gasPin = A0;  //Gas pin

    if (gps.location.isValid())
    {
      Blynk.virtualWrite(V10, String(gps.location.lat(), 6));
      Blynk.virtualWrite(V11, String(gps.location.lng(), 6));
      myMap.location(1, gps.location.lat(), gps.location.lng(), "GPS_Location");
    }

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(dht.readTemperature());
    postStr += "&field2=";
    postStr += String(dht.readHumidity());
    postStr += "&field3=";
    postStr += String(analogRead(gasPin));
    postStr += "&field4=";
    postStr += String(gps.location.lat(), 6);
    postStr += "&field5=";
    postStr += String(gps.location.lng(), 6);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop();
  delay(2000);
  
  /////////////////////////////////////////////////////////////////////////////////////////
  if (dht.readTemperature() >= 25 || analogRead(gasPin) >= 400 || dht.readHumidity() >= 50)
  {
    email(gps.location.lat(), gps.location.lng());
//    tone(buzzer, 5000);                             //send 5kHz signal
//    delay(60000);                                   //for 1 sec
//    noTone(buzzer);                                 //stop sound  //const int buzzer = 15;    //buzzer to nodeMCU pin 15
//    delay(1000);                                    //for 1 sec
  }
  /////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("--> Lat:" + String(gps.location.lat(), 6) + ", Long:" + String(gps.location.lng(), 6) + ", Temp:" + dht.readTemperature() + ", Humid:" + dht.readHumidity() + ", Gas:" + analogRead(gasPin) );
}

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);                                   //static const uint32_t GPSBaud = 9600; //If Baud rate 9600 didn't work in your case then use 4800
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  //pinMode(buzzer, OUTPUT);
}

void loop()
{
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      sendSensor();
    }
  }
  Blynk.run();
  timer.run();
}

void email(long latitude, long longitude) 
{
  String url = "Telah terjadi kebakaran di  https://www.google.com/maps/?q=" + String(latitude, 6) + "," + String(longitude, 6) + ",15z";
  Blynk.email("email@gmail.com", "Kebakaran!!!", url); //Blynk.notify("Dingin cin");
}
