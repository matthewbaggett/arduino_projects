// the setup function runs once when you press reset or power the board

int ledPins[] = {0,1,4,5,12,13,14,15,16};

void setup() {
  Serial.begin(115200);
  Serial.println("");
  delay(500);
  Serial.println("Wake up!");
  delay(500);
  for(int i = 0; i < sizeof(ledPins); i++){
      Serial.print("Setting mode on pin: ");
      Serial.println(ledPins[i]);
      delay(500);
      pinMode(ledPins[i], OUTPUT);
  }
}

// the loop function runs over and over again forever
void loop() {
  
    for(int i = 0; i < sizeof(ledPins); i++){
      Serial.print("Running on pin: ");
      Serial.println(ledPins[i]);
      digitalWrite(ledPins[i], HIGH);  
      delay(1000);              
      digitalWrite(ledPins[i], LOW);  
    }
    
}
