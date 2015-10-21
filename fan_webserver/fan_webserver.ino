int fanSpeed = 128;
int fanPin = 3;
int fanSensor = 5;
long cycle = 0;

// Libraries
#include <SPI.h>
#include <Ethernet.h>
#include <aREST.h>
#include <avr/wdt.h>

// Enter a MAC address for your controller below.
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xFE, 0x40 };

// IP address in case DHCP fails
IPAddress ip(10,0,0,4);

// Ethernet server
EthernetServer server(80);

// Create aREST instance
aREST rest = aREST();

// Variables to be exposed to the API
int temperature;
int humidity;

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
  Serial.println("Startup!");

  int sensorReading = pulseIn(fanSensor, HIGH);
  
  // Init variables and expose them to REST API
  rest.variable("fanPWMTarget", &fanSpeed);
  rest.variable("fanMesuredSpeed",&sensorReading);
  
  // Function to be exposed
  rest.function("led",ledControl);
  rest.function("setSpeed", setSpeed);
  
  // Give name and ID to device
  rest.set_id("008");
  rest.set_name("fanController");

  // Start the Ethernet connection and the server
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  // Start watchdog
  wdt_enable(WDTO_4S);

  pinMode(fanPin, OUTPUT);
}

void loop() {  
  // listen for incoming clients
  EthernetClient client = server.available();
  rest.handle(client);
  wdt_reset();
  analogWrite(fanPin, fanSpeed);
}

// Custom function accessible by the API
int ledControl(String command) {
  
  // Get state from command
  int state = command.toInt();
  
  digitalWrite(6,state);
  return 1;
}

int setSpeed(String command){
  Serial.print("Setting fanSpeed to ");
  Serial.println(command.toInt());
  fanSpeed = command.toInt();
  return fanSpeed;
}

