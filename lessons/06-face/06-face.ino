#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 6
#define SCL_PIN 7

Adafruit_SSD1306 display(128, 64, &Wire, -1);


void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true) delay(10);
  }
}

void drawEyes(int h){
    display.clearDisplay();
    int r= min(8, h/2);

    display.fillRoundRect(24, 32-h/2,28,h,r, SSD1306_WHITE);
    
    display.fillRoundRect(76, 32-h/2,28,h,r, SSD1306_WHITE);
    display.display();
}

void loop(){
    drawEyes(32);
    delay(1000);
    drawEyes(4);
    delay(1000);
}