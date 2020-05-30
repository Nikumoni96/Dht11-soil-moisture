#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <SimpleTimer.h>
#include <DHT.h>
#define DHTPIN 0          // Digital pin 3

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

char auth[] = "9ed8cb51d93d49c19173a43c563b18b3";  //code sent via email
const int sensorPin = 4; 
int sensorState = 0;
int lastState = 0;
int value= 0;
int sense_Pin= 0;


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, "vivo 1723", "12345678");  //wifi name and password
  pinMode(sensorPin, INPUT);
   dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  {
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C, Humidity: ");
  Serial.print(h);
  Serial.print("%  ");
  Serial.print("\n");
  }                
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
}

void loop()
{ 
  Blynk.run();

  sensorState = digitalRead(sensorPin);
Serial.println(sensorState);

if (sensorState == 1 && lastState == 0) {
  Serial.println("needs water, send notification");
  Blynk.notify("Water your plants");
 // lastState = 1;
  delay(10000);
//send notification
    
  } 
  else if (sensorState == 1 && lastState == 1) {
    //do nothing, has not been watered yet
  Serial.println("has not been watered yet");
  delay(10000);
  }
  else {
    //st
    Serial.println("does not need water");
    //lastState = 0;
    delay(10000);
  }
  {
    Serial.print("Moisture content in soil in Percentage:");
    value= analogRead(sense_Pin);
   value= value/10.2;
   Serial.println(value);
  }
  {
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
}
  
  
  delay(10000);
}
