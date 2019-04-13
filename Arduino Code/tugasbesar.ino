#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space 
#include <SPI.h> 
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h> 
#include <SimpleTimer.h> 
#include <DHT.h> 
#include "TinyGPS++.h" 
#include <SoftwareSerial.h> 

 
// 
//static const uint8_t D0   = 16; 
//static const uint8_t D1   = 5; 
//static const uint8_t D2   = 4; 
//static const uint8_t D3   = 0; 
static const uint8_t D4   = 2; 
//static const uint8_t D5   = 14; 
//static const uint8_t D6   = 12; 
//static const uint8_t D7   = 13; 
//static const uint8_t D8   = 15; 
//static const uint8_t D9   = 3; 
//static const uint8_t D10  = 1; 
const int gasPin = A0; 
static const int RXPin = 14, TXPin = 12; 
static const uint32_t GPSBaud = 9600; 

// You should get Auth Token in the Blynk App. 
// Go to the Project Settings (nut icon). 

  //auth 
 char auth[]="1007d8c6fc5d4d56bcc4d0b3f4ea1041"; 
 String apiKey = "2YB6KKXQFH8WAGEP";     //  Enter your Write API key from ThingSpeak 

 //wifi 
 char ssid[] = "iPhone"; //SSID 
 char pass[] = "1234567890o"; //Password 
 const char* server = "api.thingspeak.com"; 
#define DHTPIN D4       
//GN - G 
//D - 3V 
//V - D4 

 

// Uncomment whatever type you're using! 
#define DHTTYPE DHT11     // DHT 11 
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321 
//#define DHTTYPE DHT21   // DHT 21, AM2301 

WidgetMap myMap(V3); 
WiFiClient client; 
DHT dht(DHTPIN, DHTTYPE); 
SimpleTimer timer; 
TinyGPSPlus gps; 
SoftwareSerial ss(RXPin, TXPin); 

// This function sends Arduino's up time every second to Virtual Pin (5). 
// In the app, Widget's reading frequency should be set to PUSH. This means 
// that you define how often to send data to Blynk App. 
void sendSensor() 
{ 
  float h = dht.readHumidity(); 
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit 
  float g = analogRead(gasPin); 
  float lt = gps.location.lat(); 
  float ln = gps.location.lng(); 
   
   
//  Serial.println(h); 
//  Serial.println(t); 
//  Serial.println(g); 
//  Serial.println(lt); 
//  Serial.println(ln); 
   
  if (isnan(h) || isnan(t)) { 
    Serial.println("Failed to read from DHT sensor!"); 
    return; 
  } 

  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com 
                      {   
                             
                             String postStr = apiKey; 
                             postStr +="&field1="; 
                             postStr += String(t); 
                             postStr +="&field2="; 
                             postStr += String(h); 
                             postStr +="&field3="; 
                             postStr += String(g); 
                             postStr +="&field4="; 
                             postStr += String(lt); 
                             postStr +="&field5="; 
                             postStr += String(ln); 
                             postStr += "\r\n\r\n"; 
  
                             client.print("POST /update HTTP/1.1\n"); 
                             client.print("Host: api.thingspeak.com\n"); 
                             client.print("Connection: close\n"); 
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
                             client.print("Content-Type: application/x-www-form-urlencoded\n"); 
                             client.print("Content-Length: "); 
                             client.print(postStr.length()); 
                             client.print("\n\n"); 
                             client.print(postStr); 
  
                             Serial.print("Temperature: "); 
                             Serial.print(t); 
                             Serial.print(" , Humidity: "); 
                             Serial.print(h); 
                             Serial.print(" , Gas value: "); 
                             Serial.print(g); 
                             Serial.print(" , Latitude value: "); 
                             Serial.print(lt); 
                             Serial.print(" , Longitude value: "); 
                             Serial.print(ln); 
                             Serial.println("%. Send to Thingspeak."); 
                        } 
          client.stop(); 
  
          Serial.println("Waiting..."); 

          delay(10000); 
  // You can send any value at any time. 
  // Please don't send more that 10 values per second. 
  Blynk.virtualWrite(V5, h); 
  Blynk.virtualWrite(V6, t); 
  Blynk.virtualWrite(V7, g); 

  if(t >= 30 && g >=400 && h >= 50) 
  { 
    email(lt,ln); 
  } 
} 

void setup() 
{ 
  Serial.begin(9600); // See the connection status in Serial Monitor 
  ss.begin(GPSBaud); 
  Blynk.begin(auth, ssid, pass); 
  Serial.println("Connected"); 

  dht.begin(); 

  // Setup a function to be called every second 
  timer.setInterval(1000L, sendSensor); 
} 

void loop() 
{ 
  Blynk.run(); // Initiates Blynk 
  timer.run(); // Initiates SimpleTimer 

  gps.encode(ss.read()); 
  Serial.println(ss.available()); 
} 
void email(float latitude, float longitude){ 
  String stringlat = String(latitude); 
  String stringlon = String(longitude); 
  String url = "Telah terjadi kebakaran di  https://www.google.com/maps/@"+ stringlat + "," + stringlon + ",15z"; 
  Blynk.email("yohanesdwikiwitman@gmail.com", "Kebakaran!!!", url); 
  Blynk.notify("Dingin cin"); 
} 

BLYNK_WRITE(V2) { 
  GpsParam gps(param); 

//  float lat = gps.getLat(); 
//  float lon = gps.getLon(); 

  // Print 6 decimal places for Lat, Lon 
  Serial.print("Lat: "); 
  Serial.println(gps.getLat(), 7); 

  Serial.print("Lon: "); 
  Serial.println(gps.getLon(), 7); 

  // Print 2 decimal places for Alt, Speed 
  Serial.print("Altitute: "); 
  Serial.println(gps.getAltitude(), 2); 

  Serial.print("Speed: "); 
  Serial.println(gps.getSpeed(), 2); 
  myMap.location(1, gps.getLat(), gps.getLon(), "value"); 

  Serial.println(); 
}
