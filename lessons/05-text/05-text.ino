#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 6 // GPIO pin 6 is connected to the I2C data line (SDA)
#define SCL_PIN 7 // GPIO pin 7 is connected to the I2C clock line (SCL)
#define LED_PIN 8

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup(){
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    Wire.begin(SDA_PIN, SCL_PIN); // begin I2C communication on the

    while(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // start the OLED at the address 0x3C
        Serial.println(F("OLED not found"));
        delay(1000);    
    }

    display.clearDisplay();                    // wipe the screen buffer

    display.setTextSize(4);
    display.setTextColor(SSD1306_WHITE);       // without this, text is invisible
    display.setCursor(0,0);                   // top-left corner (x, y)

    display.println("meowm");
    display.display();                         // send the buffer to the screen
}


void loop(){

     digitalWrite(LED_PIN, HIGH); // OFF
    delay(1000);
    digitalWrite(LED_PIN, LOW); // ON active low
    delay(1000);
}