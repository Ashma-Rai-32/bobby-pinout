
#include <Wire.h> //arduina i2c library, helps esp32 talk to other devices

#define SDA_PIN 6 // GPIO pin 6 is connected to the I2C data line (SDA)
#define SCL_PIN 7 // GPIO pin 7 is connected to the I2C clock line (SCL)

void setup() {

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
      Serial.print("Found device at 0x");  // print without a newline
      Serial.println(address, HEX);        // print the address in hexadecimal, then newline
    }
  }

  Serial.println("Scan finished");
}

void loop() {
}
