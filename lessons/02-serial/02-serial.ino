 void setup(){
    Serial.begin(115200);
    delay(2000);

    Serial.println("Hello, world!");
 }

 void loop(){
    Serial.print("Uptime: ");
    Serial.print(millis()/1000, BIN);
    Serial.println(" seconds");
    delay(1000);
 }