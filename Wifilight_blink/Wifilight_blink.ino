// the setup function runs once when you press reset or power the board

int led = 1 ;

void setup() {
  Serial.begin(115200);
  Serial.println("");
  delay(200);
  Serial.println("Wake up!");
  delay(200);
  pinMode(led, OUTPUT);
  
}

// the loop function runs over and over again forever
void loop() {
      Serial.print("Running on pin ");
      Serial.println(led);
      digitalWrite(led, HIGH);  
      delay(250);              
      digitalWrite(led, LOW);  
      delay(250);
   
}
