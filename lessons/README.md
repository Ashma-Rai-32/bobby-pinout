# Bobby Pinout: Lessons

From "board in hand" to an animated face on the OLED. Each lesson adds one new idea.

**Hardware needed for now:** ESP32‑C3 SuperMini, 0.96" OLED, breadboard, jumper wires, USB data cable.

## Progress

- [ ] Lesson 0: Setup
- [ ] Lesson 1: Blink
- [ ] Lesson 2: Serial
- [ ] Lesson 3: Button input
- [ ] Lesson 4: Solder, breadboard and I2C scan
- [ ] Lesson 5: Text on the OLED
- [ ] Lesson 6: Draw a face
- [ ] Lesson 7: Two things at once (`millis()`)
- [ ] Lesson 8: Wi‑Fi clock

---
```
Top row:     5V   G   3.3   4   3   2   1   0
             ════════════════════════════════
   [USB]               SuperMini                [antenna]
             ════════════════════════════════
Bottom row:  5    6   7    8   9   10  20  21
```

## Lesson 0: Setup

**Goal:** get your laptop talking to the board.

### Folder layout

arduino-cli needs each sketch in a folder with the **same name** as the `.ino` file:

```
bobby-pinout/
  bobby-pinout.ino        ← main project (also what Wokwi runs)
  lessons/
    01-blink/01-blink.ino
    02-serial/02-serial.ino
    ...
```

### Find the board's port

1. Plug the SuperMini into your Mac with a **data** cable. The small red power LED should light up.
2. Run:
   ```bash
   arduino-cli board list
   ```
3. Look for the line with **`usbmodem`** in it, e.g. `/dev/cu.usbmodem1401`. Ignore any board name it guesses ("ESP32 Family Device", "Ozobot DRVKit"); it's guessing from the USB ID.

| Problem | Fix |
|---|---|
| No red LED | Board isn't getting power. Try a USB‑A to C cable through an adapter. |
| Red LED on, but no `usbmodem` port | Charge-only cable, so try another. Or hold **BOOT**, press and release **RST**, release BOOT, then check again. |

### Shell variables (`.env`)

The project's `.env` file holds two shortcuts:

```
PORT=/dev/cu.usbmodem1401
FQBN=esp32:esp32:esp32c3:CDCOnBoot=cdc
```

| Variable | Means |
|---|---|
| `PORT` | **Where** the board is plugged in. Can change if you use a different USB socket, so re-check with `arduino-cli board list` if an upload says the port wasn't found. |
| `FQBN` | **Which board** to compile for. `esp32:esp32` is the board package, `esp32c3` is the chip, and `CDCOnBoot=cdc` turns on serial over USB so `Serial.print` reaches your Mac. |

Load them once per terminal window:

```bash
source .env
echo $PORT $FQBN     # check they're set
```

`.env` should be in `.gitignore`, since your Wi‑Fi password will go somewhere like it in Lesson 8.

### Workflow for every lesson: Wokwi first, then the real board

Each lesson folder has its own `wokwi.toml` and `diagram.json` (the simulated circuit). You only create the `.ino` file, using the code below.

**1. Simulate in Wokwi**

```bash
make dev L=01-blink     # builds now, and rebuilds every time you save (Ctrl+C to stop)
```

Then in VS Code:
1. `Cmd+Shift+P` → **Wokwi: Select Config File** → pick `lessons/01-blink/wokwi.toml`
2. `Cmd+Shift+P` → **Wokwi: Start Simulator**

After each save and rebuild, restart the simulator to load the new code. When you switch lessons, select that lesson's `wokwi.toml` again. Leave out `L=` to work on the main `bobby-pinout.ino`.

**2. Upload to the real board**

Plug in the SuperMini, then:

```bash
source .env                                                    # once per terminal window
arduino-cli compile -u -p $PORT --fqbn $FQBN lessons/01-blink  # compile and upload
arduino-cli monitor -p $PORT -c baudrate=115200                # serial monitor (Ctrl+C to exit)
```

| Part | Means |
|---|---|
| `compile` | Turn your `.ino` into firmware the chip can run |
| `-u` | Upload it straight after compiling |
| `-p $PORT` | Which USB port the board is on |
| `--fqbn $FQBN` | Which board to compile for (the SuperMini, with serial over USB) |
| `lessons/01-blink` | The sketch folder |

The Wokwi build and the real-board build differ: Wokwi needs serial on the TX/RX pins (`esp32:esp32:esp32c3`), the SuperMini needs it over USB (`$FQBN`, which adds `CDCOnBoot=cdc`).

**Shortcuts:** `make upload L=01-blink` and `make monitor` run the same two commands, and read `PORT` from `.env` without `source .env`.

The **serial monitor** isn't a screen. It's a text view in your terminal that shows whatever the board sends back with `Serial.print()`, like the output panel at the bottom of Wokwi. You first need it in Lesson 2.

Close the monitor before uploading again, because the monitor and the upload can't use the port at the same time.

---

## Lesson 1: Blink

**You'll learn:** the upload workflow, `setup()` and `loop()`, output pins.
**Wiring:** none. The board has a built-in blue LED on GPIO 8.

`lessons/01-blink/01-blink.ino`:

```cpp
#define LED_PIN 8

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, LOW);   // ON: this LED is "active low"
  delay(500);
  digitalWrite(LED_PIN, HIGH);  // OFF
  delay(500);
}
```

```bash
# Wokwi
make dev L=01-blink

# real board
source .env
arduino-cli compile -u -p $PORT --fqbn $FQBN lessons/01-blink
```

**Expect:** the blue LED blinks every half second. The first compile takes a minute or two.

**In Wokwi:** the simulated ESP32‑C3 board doesn't have the SuperMini's simple LED, so the circuit adds a separate blue LED wired the same way: `3V3 → 220Ω → LED → GPIO 8`.

**Why LOW means on:** the LED is connected between 3.3V and the pin. When the pin goes LOW (0V), current flows and the LED lights.

**Try:** blink SOS in Morse code: 3 short, 3 long, 3 short.

---

## Lesson 2: Serial

**You'll learn:** printing debug messages, `millis()`.

`lessons/02-serial/02-serial.ino`:

```cpp
void setup() {
  Serial.begin(115200);
  delay(2000);  // gives your Mac time to connect, so you don't miss the first lines
  Serial.println("Hello from Bobby Pinout!");
}

void loop() {
  Serial.print("Uptime: ");
  Serial.print(millis() / 1000);
  Serial.println(" s");
  delay(1000);
}
```

```bash
# Wokwi: output shows in the simulator's serial panel
make dev L=02-serial

# real board
source .env
arduino-cli compile -u -p $PORT --fqbn $FQBN lessons/02-serial
arduino-cli monitor -p $PORT -c baudrate=115200
```

**Expect:** the monitor prints the uptime every second.

**Note:** `millis()` is the number of milliseconds since the board started. You'll lean on it in Lesson 7.

**Try:** print the uptime in binary with `Serial.println(x, BIN)`, the same trick as `HEX`.

---

## Lesson 3: Button input

**You'll learn:** input pins, pull-up resistors, reading a button.
**Wiring:** none. The **BOOT** button is on GPIO 9.

`lessons/03-button/03-button.ino`:

```cpp
#define LED_PIN 8
#define BUTTON_PIN 9

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  bool pressed = digitalRead(BUTTON_PIN) == LOW;
  digitalWrite(LED_PIN, pressed ? LOW : HIGH);
}
```

```bash
# Wokwi
make dev L=03-button

# real board
source .env
arduino-cli compile -u -p $PORT --fqbn $FQBN lessons/03-button
```

**Expect:** the LED is on while you hold BOOT.

**In Wokwi:** the circuit adds a green button labelled BOOT, wired from GPIO 9 to GND like the real one, plus the blue LED from Lesson 1. Click and hold it with the mouse.

**Why LOW means pressed:** `INPUT_PULLUP` holds the pin at 3.3V (HIGH) through an internal resistor. Pressing the button connects the pin to ground, so it reads LOW.

**Don't hold BOOT while pressing RST or plugging in.** That starts the board in upload mode instead of running your code.

**Try:** make each press **toggle** the LED. You'll notice one press sometimes counts as several. That's **bounce**: the metal contacts rattle for a few milliseconds. Fix it by ignoring presses that come within 50 ms of the last one.

---

## Lesson 4: Solder, breadboard and I2C scan

**You'll learn:** soldering headers, breadboard layout, real I2C.

1. **Solder the headers.** Push the long ends of the pins into the breadboard so it holds them straight, put the board on top, and solder the short ends. Max 2–3 seconds per pin, and check for **solder bridges** (blobs joining neighbouring pins).
2. **Seat the SuperMini across the centre gap** of the breadboard. Each row of 5 holes is connected inside, so on one side the pins on opposite edges would be shorted together.
3. **Wire the OLED with USB unplugged.** Go by the labels printed on *your* OLED, since some have GND and VDD swapped.

| OLED | SuperMini |
|---|---|
| GND | G |
| VDD | 3.3 (**not** 5V) |
| SCK | 7 |
| SDA | 6 |

4. Upload the main sketch, which scans the bus before drawing:
   ```bash
   source .env
   arduino-cli compile -u -p $PORT --fqbn $FQBN .
   arduino-cli monitor -p $PORT -c baudrate=115200
   ```
   The `.` means "the current folder", i.e. the main `bobby-pinout.ino`.
   Press **RST** after the monitor opens to see the scan from the start.

**Expect:** `Found device at 0x3C`, the same as in Wokwi, but now on real hardware.

**In Wokwi:** this lesson uses the main project (`make dev` with no `L=`, and the `wokwi.toml` in the project root), which you've already simulated. The new part here is the physical wiring.

**If nothing is found:** SDA and SCK are probably swapped, or a jumper wire isn't fully seated.

**Safety check when you plug in:** touch the chip and the OLED in the first few seconds. If either gets hot, unplug immediately; that means a short or reversed power.

---

## Lesson 5: Text on the OLED

**You'll learn:** using a library, the display buffer.

`lessons/05-text/05-text.ino`:

```cpp
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

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hello!");
  display.display();  // nothing appears until you call this
}

void loop() {}
```

```bash
# Wokwi
make dev L=05-text

# real board
source .env
arduino-cli compile -u -p $PORT --fqbn $FQBN lessons/05-text
arduino-cli monitor -p $PORT -c baudrate=115200
```

**Why `display.display()` matters:** every drawing command only changes a copy of the screen in the ESP32's memory, called the **buffer**. `display()` sends the whole buffer over I2C in one go. That's how animation works without flicker.

**Try:** show your name, then a live uptime counter underneath it.

---

## Lesson 6: Draw a face

**You'll learn:** pixel coordinates, simple animation.

`lessons/06-face/06-face.ino` (start with the includes, `display` object and `setup()` from Lesson 5):

```cpp
void drawEyes(int h) {
  display.clearDisplay();
  int r = min(8, h / 2);
  display.fillRoundRect(24, 32 - h / 2, 28, h, r, SSD1306_WHITE);
  display.fillRoundRect(76, 32 - h / 2, 28, h, r, SSD1306_WHITE);
  display.display();
}

void loop() {
  drawEyes(32);   // open
  delay(2500);
  drawEyes(4);    // blink
  delay(120);
}
```

```bash
# Wokwi
make dev L=06-face

# real board
source .env
arduino-cli compile -u -p $PORT --fqbn $FQBN lessons/06-face
```

**Coordinates:** (0, 0) is the **top-left** corner. x goes right up to 127, and y goes **down** to 63.

**Try:**
- Make the eyes look left and right by shifting their x position.
- Blink at random intervals with `random(1500, 4000)`.
- Add a smile with `display.drawCircle()` or `drawLine()`.

---

## Lesson 7: Two things at once (`millis()` instead of `delay()`)

**You'll learn:** the most important embedded pattern there is.

In Lesson 6, the board is stuck inside `delay(2500)`, so a button press during that time is missed. The fix is to check the clock instead of waiting.

`lessons/07-millis/07-millis.ino` (start from your Lesson 6 code, and add `pinMode(9, INPUT_PULLUP);` to `setup()`):

```cpp
unsigned long lastBlink = 0;

void loop() {
  if (millis() - lastBlink >= 2500) {
    lastBlink = millis();
    drawEyes(4);
    delay(120);       // a short wait is fine
    drawEyes(32);
  }

  if (digitalRead(9) == LOW) {
    // switch face, show text, anything
  }
}
```

```bash
# Wokwi
make dev L=07-millis

# real board
source .env
arduino-cli compile -u -p $PORT --fqbn $FQBN lessons/07-millis
```

**Try:** use BOOT to cycle between 3 faces (normal, happy, sleepy) while the blinking keeps going.

**In Wokwi:** the circuit has the OLED plus the green BOOT button on GPIO 9.

---

## Lesson 8: Wi‑Fi clock

**You'll learn:** Wi‑Fi, getting the time from the internet (NTP).

`lessons/08-wifi-clock/08-wifi-clock.ino` (start with the includes, `display` object and `setup()` from Lesson 5):

```cpp
#include <WiFi.h>
#include <time.h>

// in setup(), after the display starts:
WiFi.begin("YOUR_WIFI", "YOUR_PASSWORD");
while (WiFi.status() != WL_CONNECTED) delay(200);
configTime(5 * 3600 + 45 * 60, 0, "pool.ntp.org");  // Nepal is UTC+5:45

// in loop():
struct tm t;
if (getLocalTime(&t)) {
  char buf[9];
  strftime(buf, sizeof buf, "%H:%M:%S", &t);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(16, 24);
  display.print(buf);
  display.display();
}
delay(1000);
```

```bash
# Wokwi
make dev L=08-wifi-clock

# real board
source .env
arduino-cli compile -u -p $PORT --fqbn $FQBN lessons/08-wifi-clock
arduino-cli monitor -p $PORT -c baudrate=115200
```

**In Wokwi:** use the simulator's built-in network: `WiFi.begin("Wokwi-GUEST", "");` (no password). Switch to your real Wi‑Fi name and password before uploading to the board.

**If it never connects:**
- Some SuperMini boards have a weak antenna. Add `WiFi.setTxPower(WIFI_POWER_8_5dBm);` right after `WiFi.begin(...)`.
- The ESP32‑C3 only supports **2.4 GHz** Wi‑Fi, not 5 GHz.

**Don't commit your Wi‑Fi password.** Put it in a `secrets.h` file next to the sketch and add that file to `.gitignore`.

---

## Reference

### SuperMini pins (USB on the left)

```
Top row:     5V   G   3.3   4   3   2   1   0
   [USB]               SuperMini                [antenna]
Bottom row:  5    6   7    8   9   10  20  21
```

Every numbered pin is a **GPIO**: your code decides whether it reads (button, sensor) or writes (LED, signal).

**Power pins**

| Pin | What it is |
|---|---|
| 5V | 5V **out** on USB. Later, the battery feeds power **in** here, through the switch. |
| G | Ground (0V). Every circuit needs a wire back to G. |
| 3.3 | 3.3V **out** from the regulator. Powers the OLED and sensors. Never connect 5V to it. |

**GPIO pins**

| Pin | Extra abilities | Notes |
|---|---|---|
| 0 | Analog input, deep-sleep wake | Free |
| 1 | Analog input, deep-sleep wake | Free |
| 2 | Analog input, deep-sleep wake | ⚠️ Strapping pin: don't pull LOW at power-on |
| 3 | Analog input, deep-sleep wake | Free |
| 4 | Analog input, deep-sleep wake | Free |
| 5 | Analog input*, deep-sleep wake | *Analog doesn't work while Wi‑Fi is on |
| 6 | | **OLED SDA** |
| 7 | | **OLED SCK** |
| 8 | | **Blue LED** (active low). ⚠️ Strapping pin |
| 9 | | **BOOT button**. ⚠️ Strapping pin |
| 10 | | Free |
| 20 | Serial RX | Free unless using a hardware serial device |
| 21 | Serial TX | Free unless using a hardware serial device |

- **Analog input:** reads 0–3.3V as a number (0–4095) instead of just HIGH or LOW. E.g. battery level, light sensor.
- **Deep-sleep wake:** only pins 0–5 can wake the board from deep sleep, so a wake-up button goes on one of them.
- **Strapping pin:** the chip checks 2, 8 and 9 at power-on to decide how to boot. Holding them in the wrong state starts upload mode instead of your code (that's what holding BOOT does).
- **RX/TX:** a second serial port for modules like GPS. `Serial.print` uses USB instead, so these are free.
- **Any GPIO** can do PWM (LED dimming), and I2C or SPI can go on any pins. That's why the OLED can use 6 and 7.
- Numbers skip 11–19 because those pins are used inside the board (memory chip and USB).

### Before you plug in

- Wire with USB **unplugged**.
- OLED VDD to **3.3**, never 5V.
- Never connect 5V or 3.3 directly to G.
- Don't rest the board on metal.
- Use a 220–330 Ω resistor with any LED you add.

### Tip

After each lesson works, commit it:

```bash
git add . && git commit -m "lesson 1: blink"
```

## Next (when the battery and switch arrive)

Battery wiring with the TP4056 and the switch, sleep modes, and battery life.
