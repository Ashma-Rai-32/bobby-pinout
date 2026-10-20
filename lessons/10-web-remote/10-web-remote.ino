#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_SSD1306.h>
#include "secrets.h"
#include "page.h"

#define SDA_PIN 6
#define SCL_PIN 7
#define LED_PIN 8

Adafruit_SSD1306 display(128, 64, &Wire, -1);
WebServer server(80);   // port 80 = the normal port for web pages

void showText(String text, int size) {
  display.clearDisplay();
  display.setTextSize(size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(text);
  display.display();
}

void handleRoot() {
  server.send(200, "text/html", PAGE);
}

bool ledOn=false;
void handleToggle(){
    ledOn=!ledOn;
    digitalWrite(LED_PIN, ledOn?LOW:HIGH); //on/off
    server.send(200, "text/plain", ledOn?"ON":"OFF");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true) delay(10);
  }

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); //off

  showText("Connecting...", 1);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);   // helps the SuperMini's weak antenna
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  String ip = WiFi.localIP().toString();
  Serial.println("\nOpen http://" + ip);
  showText("Open:\n\nhttp://" + ip, 1);   // the IP can change, so always show it

  server.on("/", handleRoot);   // "/" = the main page
  server.on("/toggle", handleToggle); 
  server.begin();
}

void loop() {
  server.handleClient();   // check for requests from browsers
}
