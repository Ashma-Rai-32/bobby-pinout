
#include <Wire.h> //arduina i2c library, helps esp32 talk to other devices
#include <Adafruit_GFX.h>      // drawing: text, shapes, pixels
#include <Adafruit_SSD1306.h>  // driver for your OLED chip


#define SDA_PIN 6 // GPIO pin 6 is connected to the I2C data line (SDA)
#define SCL_PIN 7 // GPIO pin 7 is connected to the I2C clock line (SCL)
Adafruit_SSD1306 display(128, 64, &Wire, -1); // width, height, I2C bus, no reset pin

void setup() {

  int foundAddress=0;

  Serial.begin(115200); // baud rate, sends one 115200 bits per second to the serial monitor

  Wire.begin(SDA_PIN, SCL_PIN); // begin I2C communication on the specified SDA and SCL pins

  // Every I2C device has a 7-bit address (1-126 are usable).
  // Try each one and see if anything answers.
  for (int address = 1; address < 127; address++) {
    // Begin a message to this address...
    Wire.beginTransmission(address);

    // ...and immediately end it. endTransmission() returns 0 if a device
    // acknowledged (ACK) the address, i.e. something lives there.
    if (Wire.endTransmission() == 0) {
      foundAddress=address;
      Serial.print("Found device at 0x");  // print without a newline
      Serial.println(address, HEX);        // print the address in hexadecimal, then newline
    }
  }

  Serial.println("Scan finished:");
  Serial.println(foundAddress, HEX); // print the address in hexadecimal, then newline
  
  
  
  
  display.begin(SSD1306_SWITCHCAPVCC, foundAddress); // start the OLED at the address the scan found
  display.invertDisplay(true);   // white background, dark text
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(50);   // 0 = dimmest, 255 = brightest

  display.clearDisplay();                    // wipe the screen buffer
  display.setTextColor(SSD1306_WHITE);       // without this, text is invisible
  display.setTextSize(4);                    // 1 = tiny, 3 = big
  display.setCursor(0,0);                   // top-left corner (x, y)

  const char* word="GRMLN";

  for (int i =0; word[i]!='\0'; i++) {
    display.setCursor(25*i, 15);                   // top-left corner (x, y)
    display.print(word[i]);  
    display.display();                         // send the buffer to the screen
    delay(2000); // wait 200 milliseconds
  }
 
}

void loop() {
}
