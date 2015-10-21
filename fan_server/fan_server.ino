int fanSpeed = 128;
int fanPin = 3;
int fanSensor = 5;
long cycle = 0;
long line = 0;
bool state;

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10, 0, 0, 4);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  pinMode(fanPin, OUTPUT);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  cycle++;
  if(cycle < 0){
    cycle = 0;
  }
  analogWrite(fanPin, fanSpeed);
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    line = 0;
    while (client.connected()) {
     
      line++;
      
      if (client.available()) {
        char c = client.read();
        Serial.write(line);
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          int sensorReading = pulseIn(fanSensor, HIGH);
          // send a standard http response header
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response

          client.println();
          client.println("{");
          
          client.print(" \"fanSpeed\":  ");
          client.print(sensorReading);
          client.println(",");

          client.print(" \"fanPWMTarget\":  ");
          client.print(fanSpeed);
          client.println(",");

          client.print(" \"cpuCycle\":  ");
          client.print(cycle);
          client.println("");

          
          
          client.println("}");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");

    state = !state;
    

    if(state){
      Serial.println("State A ON");
      digitalWrite(fanPin, 0);
    }else{
      Serial.println("State B OFF");
      digitalWrite(fanPin, 1);
    }
    
  }
}

