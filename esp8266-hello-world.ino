//iotapp - ESP8266 Hello World template

//Dependencies
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define LED 2

 String wifiName = "WifiName"; // Your WiFi network handle
 String wifiPass = "password"; // Your WiFi network password
 String urlKey = "MY_URL_KEY"; // Your iotapp URL key
 long randNumber; // random integer 
  
 
void setup() {
  Serial.begin(115200);  //Serial connection
  WiFi.begin(wifiName, wifiPass);   //Connect the wifi
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFi connection completion
     pinMode(LED, OUTPUT);  // declare LED as output
    delay(500);
    Serial.println("Waiting for connection");
  }
}
 
void loop() {

  
 randNumber = random(300);
 Serial.println(randNumber);
  delay(50);

 
if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
  JsonObject& JSONencoder = JSONbuffer.createObject();

  JSONencoder["value"] = randNumber;
  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
 
   HTTPClient http;    //Declare httpClient
 
   http.begin(apiKey); //Pass apiKey as the connection string
   http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
 int httpCode = http.POST(JSONmessageBuffer);   //Send the request
 String payload = http.getString();        //Get the response payload

  //flash LED 2 when sending request
  digitalWrite(LED, LOW);
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(1000);
  
   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payload
   http.end();  //Close connection
 
 }else{
    Serial.println("Connection Error");  //else there's an error
 }
  delay(30000);  //Time between requests (Milliseconds)
}
