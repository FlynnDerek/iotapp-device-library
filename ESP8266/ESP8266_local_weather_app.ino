/* iotapp - ESP8266 - Local Weather App Sketch - Using a standard ESP32 and BME280 sensor, we can get the local
temperature, humidity, pressure and altitude. The sensor readings are read and posted via HTTPS to the iotapp console. */

// Dependencies
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ArduinoJson.h>

//  Insert your Wifi credentials, then your given channel URL in the iotapp console. 
 String wifiName = "YOUR_WIFI_NAME"; 
 String wifiPass = "YOUR_WIFI_PASS";
 String urlKey = "YOUR_IOTAPP_URL"; // Example: https://iotappbe.com:5001/api/DyUlKq8quK7G9lWQsEwXvGTROR4rv5Yo
 const char* fingerprint = "b1 8d c5 3c a6 e4 e2 6b 68 20 64 88 bc 3b dd fe af 22 51 12"; //iotapp fingerprint

// Use this configuration if the sensor is using SPI communication protocol
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5
Adafruit_BME280 bme(BME_CS); // hardware SPI
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
*/

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C

void setup() {
  // Connect to WiFi
  Serial.begin(115200);
  WiFi.begin(wifiName, wifiPass);   //Connect the WiFi
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFi to connect
    delay(500);
    Serial.println("Waiting for connection");
  }
  bool status;
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could'nt find BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() { 
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    
  StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
  JsonObject& JSONencoder = JSONbuffer.createObject(); //Create new JSON object

  // Get the sensor readings
  String temperature = String(1.8 * bme.readTemperature() + 32); //Get the temperature
  String humidity = String(bme.readHumidity()); //Get the humidity
  // String altitude = String(bme.readAltitude(SEALEVELPRESSURE_HPA)); //Get the altitude
  // String pressure = String(bme.readPressure() / 100.0F); //Get the pressure

  //You can format how the data is displayed inside the string below. 
  JSONencoder["value"] = String("Temperature = " + temperature + " *F" + ", Humidity = " + humidity + " %");
  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

   HTTPClient http; 
   http.begin(urlKey, fingerprint);  //Pass fingerprint and URL key
   http.addHeader("Content-Type", "application/json"); //Content style should always be "application/json"
   int httpResponseCode = http.POST(JSONmessageBuffer);  //Send the POST request
   if(httpResponseCode>0){
    String response = http.getString(); //Get the response 
    Serial.println(httpResponseCode);   //Print HTTP return code
    Serial.println(response);           //Print the response
   }else{
    Serial.print("Error on sending POST: "); //If error, print. 
    Serial.println(httpResponseCode);
   }
   http.end();  //Free resources
 }else{
    Serial.println("Error in WiFi connection");   
 }
  delay(30000);  //Specify request interval, set at 30000 milliseconds, or 30 seconds. 
}
