//iotapp - ESP8266 - Hello World Sketch - This program generates a random number and posts via HTTPS to the iotapp console. 

//Dependencies
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

//  Insert your Wifi credentials, then your given channel URL in the iotapp console. 
 String wifiName = "YOUR_WIFI_NAME"; 
 String wifiPass = "YOUR_WIFI_PASS";
 String urlKey = "YOUR_IOTAPP_URL"; // Example: https://iotappbe.com:5001/api/DyUlKq8quK7G9lWQsEwXvGTROR4rv5Yo
 const char* fingerprint = "b1 8d c5 3c a6 e4 e2 6b 68 20 64 88 bc 3b dd fe af 22 51 12"; //iotapp fingerprint

  
#define LED 2 //initalize LED 2
long randNumber = 0; // initalize random integer 
 
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

 randNumber = random(300); //initialize a random number
 Serial.println(randNumber); //print random number
 delay(50);

if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
  StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
  JsonObject& JSONencoder = JSONbuffer.createObject();

  JSONencoder["value"] = randNumber;
  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
 
   HTTPClient http;    //Declare httpClient
 
   http.begin(urlKey, fingerprint); //Pass args
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
