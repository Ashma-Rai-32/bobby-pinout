#include <WiFi.h>
#include <time.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 6 // GPIO pin 6 is connected to the I2C data line (SDA)
#define SCL_PIN 7 // GPIO pin 7 is connected to the I2C clock line (SCL)


Adafruit_SSD1306 display(128, 64, &Wire, -1);


float additionalOffset= 5*3600+45*60;

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    

    Serial.println("Connecting to WiFi...");
    
    configTime(additionalOffset, 0, "pool.ntp.org");
    WiFi.begin("wudalchi", "CLFE2D9B92");
    WiFi.setTxPower(WIFI_POWER_8_5dBm);

    // WiFi.begin("Wokwi-GUEST", "");

    while(WiFi.status()!= WL_CONNECTED){
        Serial.print(".");
    delay(200);
    }

    Serial.println("Connected to WiFi");
}

struct tm t;


void loop() {
if (getLocalTime(&t)){

    char buffer[80];

    strftime(buffer, sizeof buffer, "%H:%M:%S", &t);
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print(buffer);
    display.display();
}
}