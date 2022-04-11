#include <WiFi.h>
#include <HTTPClient.h>
#include <sstream>
struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};
const char * messagingServiceSid = "MGc8853ff5f95ec94f196c69d05a7653e9";
const char * to = "+917974471702";
const char * accountNr = "AC6b2188bd2c418ffb621c0251beb96d27";
const char * twilioPassword = "185ed13be88a3564cda7ce4dfedafc99";
bool sendSMS(const char * body){
 
  std::stringstream url;
  url << "https://api.twilio.com/2010-04-01/Accounts/" << accountNr <<"/Messages";
 
  std::stringstream urlEncodedBody;
  urlEncodedBody << "MessagingServiceSid=" << messagingServiceSid << "&To=" << to << "&Body=" << body;
 
  Serial.print("\nURL: ");
  Serial.println(url.str().c_str());
  Serial.print("Encoded body: ");
  Serial.println(urlEncodedBody.str().c_str());
   
 
  HTTPClient http;
 
  http.begin(url.str().c_str());
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.setAuthorization(accountNr, twilioPassword);
   
  int httpCode = http.POST(urlEncodedBody.str().c_str());                                               
  
  if (httpCode > 0) {
 
      String payload = http.getString();
 
      Serial.print("\nHTTP code: ");
      Serial.println(httpCode);
 
      Serial.print("\nResponse: ");
      Serial.println(payload);
    }
 
  else {
    Serial.println("Error on HTTP request:");
    Serial.println(httpCode);
  }
 
  http.end();
 
  return httpCode == 201;
 
}

Button button1 = {35, 0, false};

void IRAM_ATTR isr() {
   //Detach Interrupt after 1 Minute
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 250) {
    lastMillis = millis();
   button1.numberKeyPresses += 1;
  button1.pressed = true;
  }
}
const char* ssid = "Anfaas";
const char* password =  "Anfaas@123";


String server = "https://heroku-boot-123.herokuapp.com/hello";

unsigned long last_time = 0;
unsigned long timer_delay = 10000;

void setup() {
  Serial.begin(115200); 
pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WIFI..");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  // Random number generator
}

void loop() {
 
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String server_path = server ;
      http.begin(server_path.c_str());
      if (button1.pressed) {
      Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
      button1.pressed = false;
  
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String data = http.getString();
        Serial.println(data);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
  
      http.end();
        bool result = sendSMS("your _____ needs emergency pressed emergency button");
  if(result){
    Serial.println("\nSMS sent");
  }else{
    Serial.println("\nSMS failed");
  }
      }
    }
    else {
      Serial.println("WiFi is Disconnected!");
    }
    last_time = millis();
  
}
