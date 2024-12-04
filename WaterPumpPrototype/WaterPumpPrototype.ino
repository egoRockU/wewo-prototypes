int rPump = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(rPump, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() == 0){
    String response = Serial.readString();
    Serial.println(response);
    if (response) {
      response.trim();
      int separatorIndex = response.indexOf(":");
      if (separatorIndex != -1) {
        String label = response.substring(0, separatorIndex + 1); 
        String value = response.substring(separatorIndex + 1);

        // Remove extra whitespace
        label.trim(); 
        value.trim();
        int response_val = value.toInt();
        
        digitalWrite(rPump, HIGH);
        int duration = response_val * 1000;
        delay(duration);
        digitalWrite(rPump, LOW);
      }
    }
  }
}

