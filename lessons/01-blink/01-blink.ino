#define LED_PIN 8

void setup(){
    pinMode(LED_PIN, OUTPUT);
}

void loop(){
    digitalWrite(LED_PIN, HIGH); // OFF
    delay(1000);
    digitalWrite(LED_PIN, LOW); // ON active low
    delay(1000);
}