/* iotapp - ESP8266 - Home/Building Security App - Using a generic ESP8266 module and PIR Motion sensor, we can detect
movement up to a 110 degree angle in a 20 meter area. The sensor readings are read and posted via HTTPS to your channel in the
iotapp console. */

//Dependencies
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

// Insert your Wifi credentials, then your given channel URL in the iotapp console. 
const char* wifiName = "YOUR_WIFI_NAME";
const char* wifiPass =  "YOUR_WIFI_PASSWORD";
String urlKey = "YOUR_IOTAPP_CHANNEL_URL"; // Example: https://iotappbe.com:5001/api/DyUlKq8quK7G9lWQsEwXvGTROR4rv5Yo
const char* fingerprint = "b1 8d c5 3c a6 e4 e2 6b 68 20 64 88 bc 3b dd fe af 22 51 12"; //iotapp fingerprint

// Movement alert to be pushed to the iotapp console (Example: "Movement Detected - Hallway")
String Status = "Movement Detected";

int pin = 13;  // Digital pin D7

void setup() {
  pinMode(pin, INPUT);
  Serial.begin(115200);
  WiFi.begin(wifiName, wifiPass);   //Connect the wifi
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFi connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
}

void loop() {
  bool isDetected = digitalRead(pin);
  if(isDetected){
    Serial.println("Movement Detected");
    pushToConsole();    
  } else {
    Serial.println("No Movements...");
  } 
  delay(3000);
}

void pushToConsole() {
  
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  
    StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
    JsonObject& JSONencoder = JSONbuffer.createObject(); //Create new JSON object
    JSONencoder["value"] = String(Status);
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
      } else{
        Serial.print("Error on sending POST: "); //If error, print. 
        Serial.println(httpResponseCode);
      }
        http.end();  //Free resources
      }else{
     Serial.println("Error in WiFi connection");   
   }
}
