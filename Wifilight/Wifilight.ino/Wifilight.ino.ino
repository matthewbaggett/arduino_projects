const char* ssid = "Greynet";
const char* password = "CaptainMorgan";
const char* hostname_ = "WifiLightMk4";

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 

MDNSResponder mdns;
//ADC_MODE(ADC_VCC);

ESP8266WebServer server(80);

const int led = 15;
const int sda = 12;
const int scl = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  delay(1);
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
 
void setup(void){
  
  pinMode(led, OUTPUT);
  Wire.begin(sda, scl);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin(hostname_, WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", handleRoot);

  server.on("/state", [](){
    String json = "{";
    json += "\"ChipID\": \"" + String(ESP.getChipId()) + "\",\n";
    json += "\"FlashID\": \"" + String(ESP.getFlashChipId()) + "\",\n";
    json += "\"FlashSize\": \"" + String(ESP.getFlashChipSize()/1024) + "K\",\n";
    json += "\"FlashSpeed\": \"" + String(ESP.getFlashChipSpeed()/1000) + " khz\",\n";
    //json += "\"Voltage\": " + String(ESP.getVcc()) + "\n";
    json += "\"Version\": \"" + Version + "\"\n";
    json += "}";
    server.send(200, "application/json", json);
  });

  server.on("/led-on", [](){
    digitalWrite(led, 1);  
    server.send(200, "text/plain", "Light on");
  });

  server.on("/led-off", [](){
    digitalWrite(led, 1);  
    server.send(200, "text/plain", "Light on");
  });

  
  
  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 
