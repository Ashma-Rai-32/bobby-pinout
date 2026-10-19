#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "frames.h"

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

 int frameIndex = 0;
 void loop(){
    display.clearDisplay();

    display.drawBitmap(0,0, epd_bitmap_allArray[frameIndex], 128, 64, SSD1306_WHITE);
    display.display();

    frameIndex = (frameIndex + 1) % epd_bitmap_allArray_LEN;
    delay(100);
 }