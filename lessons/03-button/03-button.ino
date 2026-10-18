#define LED_PIN 8
#define BUTTON_PIN 9

void setup(){
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}


void loop(){
    bool pressed= digitalRead(BUTTON_PIN)==LOW; // active low
    digitalWrite(LED_PIN, pressed? LOW:HIGH); // ON when pressed, OFF when not pressed
    delay(1000);
}