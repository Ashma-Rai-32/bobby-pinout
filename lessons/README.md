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
- [ ] Lesson 10: Phone remote (web interface)

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

## Lesson 10: Phone remote (web interface)

**You'll learn:** running a web server on the ESP32, HTML pages, URLs with parameters, and controlling hardware from a browser.

**Goal:** open a page on your phone, tap buttons or type a message, and the board reacts.

You build **one sketch in small steps**. Each step adds one idea; test it before moving on.

```
Phone browser                          ESP32
┌──────────────────────┐               ┌───────────────────────┐
│ http://192.168.1.42  │  GET /toggle  │ WebServer sees /toggle│
│ [ Toggle LED ]       │  ──────────►  │ → runs handleToggle() │
│ LED ON               │  ◄──────────  │ → replies "LED ON"    │
└──────────────────────┘   reply text  └───────────────────────┘
```

### Folder layout

```
lessons/10-web-remote/
├── 10-web-remote.ino   ← Wi‑Fi, server, handlers (the C++ logic)
├── page.h              ← the web page's HTML (from step 2)
├── secrets.h           ← Wi‑Fi name and password (git-ignored)
├── wokwi.toml          ← includes a port forward (see "In Wokwi")
└── diagram.json        ← copy from lessons/05-text/ (OLED + blue LED on GPIO 8)
```

`secrets.h` (use `Wokwi-GUEST` and `""` in the simulator):

```cpp
#define WIFI_SSID "your-wifi-name"
#define WIFI_PASS "your-wifi-password"
```

Use `WIFI_SSID`, not just `SSID`: the WiFi library has its own `WiFi.SSID()`, and a `#define SSID` would replace that too. Check `secrets.h` is in the project's `.gitignore` and that `git status` doesn't list it.

### Where should the HTML live?

| Option | How | Verdict |
|---|---|---|
| Raw string in the `.ino` | `const char PAGE[] = R"rawliteral( ... )rawliteral";` above `setup()` | Works, but the `.ino` gets long and mixes two languages |
| **Raw string in `page.h`** | Same string, in its own file, `#include "page.h"` | ✅ **Use this.** Keeps HTML and C++ apart, still uploads with your code, same pattern as `secrets.h` and `frames.h` |
| `index.html` on the ESP32's filesystem (LittleFS) | A real `.html` file uploaded separately to flash | What bigger projects do, with proper HTML editing, but it needs a separate filesystem upload step. Worth it once the page has several files (CSS, JS, images). |

**Why `R"rawliteral(...)rawliteral"`?** HTML is full of `"` quotes. In a normal C++ string, every one would need a `\` in front. A **raw string** takes everything between `R"rawliteral(` and `)rawliteral"` exactly as written. `rawliteral` is just a label; any word works, as long as it doesn't appear inside the HTML.

---

### Step 1: Hello from the ESP32

**New idea:** a web server, and one URL path linked to one function.

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_SSD1306.h>
#include "secrets.h"

#define SDA_PIN 6
#define SCL_PIN 7

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
  server.send(200, "text/plain", "Hello from Bobby!");
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true) delay(10);
  }

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
  server.begin();
}

void loop() {
  server.handleClient();   // check for requests from browsers
}
```

**Test:** open the address from the OLED. The browser shows `Hello from Bobby!`.

**What happened:**
- `server.on("/", handleRoot)` links the path `/` to the function `handleRoot`.
- `server.handleClient()` in `loop()` checks for visitors. Without it, the page never loads.
- `server.send(200, "text/plain", ...)` replies. **200** means "OK" (404 means "not found"), and **`text/plain`** tells the browser it's plain text.

---

### Step 2: A real page with a button

**New ideas:** HTML in `page.h`, and a second path for an action.

Create **`page.h`**:

```cpp
const char PAGE[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Bobby Remote</title>
  <style>
    body   { font-family: sans-serif; text-align: center; margin-top: 40px; }
    button { padding: 12px 24px; font-size: 18px; }
  </style>
</head>
<body>
  <h1>Bobby Remote</h1>
  <button onclick="toggleLED()">Toggle LED</button>

  <script>
    function toggleLED() {
      fetch('/toggle');
    }
  </script>
</body>
</html>
)rawliteral";
```

In the `.ino`:

1. Add `#include "page.h"` under `#include "secrets.h"`.
2. Add `#define LED_PIN 8`, and a variable above `setup()`: `bool ledOn = false;`
3. In `setup()`, before Wi‑Fi: `pinMode(LED_PIN, OUTPUT);` and `digitalWrite(LED_PIN, HIGH);` (off, since it's active low).
4. Change `handleRoot` to send the page as **HTML**:
   ```cpp
   void handleRoot() {
     server.send(200, "text/html", PAGE);
   }
   ```
5. Add the toggle handler, and link it in `setup()` with `server.on("/toggle", handleToggle);`:
   ```cpp
   void handleToggle() {
     ledOn = !ledOn;
     digitalWrite(LED_PIN, ledOn ? LOW : HIGH);   // active low
     server.send(200, "text/plain", ledOn ? "LED ON" : "LED OFF");
   }
   ```

**Test:** tap **Toggle LED**. The blue LED switches on and off.

**What happened:**
- `"text/html"` tells the browser to **draw** the reply as a page, instead of showing it as text.
- **`<meta name="viewport" ...>`** makes the page fit a phone screen instead of looking zoomed out.
- **`fetch('/toggle')`** is JavaScript asking the ESP32 for `/toggle` **in the background**, without leaving the page.
- Name the variable after **what it means** (`ledOn`), so it doesn't clash with step 5's screen invert.

---

### Step 3: Show the board's reply on the page

**New idea:** reading the reply in JavaScript.

The ESP32 already replies `LED ON` or `LED OFF`, but the page ignores it. In `page.h`, add a paragraph under the button:

```html
<p id="reply"></p>
```

and replace the script with a reusable `send()` function:

```html
<script>
  function send(path) {
    fetch(path)
      .then(r => r.text())
      .then(t => document.getElementById('reply').textContent = t);
  }
</script>
```

Change the button to use it: `<button onclick="send('/toggle')">Toggle LED</button>`

**Test:** the page now shows `LED ON` or `LED OFF` under the button.

**What happened:** `fetch` returns the reply; `.then(r => r.text())` reads it as text; the next `.then` puts it into the element with `id="reply"`. From now on, every button is just `send('/some-path')`.

---

### Step 4: Send a message to the OLED

**New idea:** URL **parameters**, extra data after a `?`.

Add a handler, and link it with `server.on("/msg", handleMsg);`:

```cpp
void handleMsg() {
  String text = server.arg("text");   // the value after ?text=
  showText(text, 2);
  server.send(200, "text/plain", "Showing: " + text);
}
```

**Test it before building any page:** type this straight into the browser's address bar:

```
http://192.168.1.42/msg?text=hello
```

`hello` appears on the OLED. That's all a button does behind the scenes: it asks for a URL.

Now add a text box and button to `page.h`, above the reply paragraph:

```html
<input id="msg" placeholder="Type a message">
<button onclick="sendMsg()">Show on screen</button>
```

and this function inside `<script>`:

```js
function sendMsg() {
  const text = document.getElementById('msg').value;
  send('/msg?text=' + encodeURIComponent(text));
}
```

**What happened:** `encodeURIComponent` makes text safe for a URL. Spaces and symbols like `&` or `?` would otherwise break it (a space becomes `%20`).

Text size 2 fits about 10 characters per line and 4 lines. Use `showText(text, 1)` for longer messages.

---

### Step 5: More controls

**New idea:** the pattern. Every feature is **one handler + one `server.on` + one button**.

```cpp
bool screenInverted = false;

void handleInvert() {
  screenInverted = !screenInverted;
  display.invertDisplay(screenInverted);
  server.send(200, "text/plain", screenInverted ? "Inverted" : "Normal");
}

void handleClear() {
  display.clearDisplay();
  display.display();
  server.send(200, "text/plain", "Cleared");
}
```

```cpp
server.on("/invert", handleInvert);
server.on("/clear", handleClear);
```

```html
<button onclick="send('/invert')">Invert screen</button>
<button onclick="send('/clear')">Clear screen</button>
```

**Make it look like an app** by adding to the `<style>` in `page.h`:

```css
body   { background: #111; color: #eee; padding: 16px; }
input, button { width: 100%; box-sizing: border-box; margin: 6px 0; font-size: 18px; padding: 12px; }
button { background: #333; color: #eee; border: 1px solid #555; border-radius: 8px; }
```

On the phone, use **Share → Add to Home Screen** to open it like an app.

---

### Step 6: Make it reliable

**New ideas:** a name instead of an IP, and reconnecting by itself.

**A name instead of a number:** add `#include <ESPmDNS.h>`, and after the Wi‑Fi `while` loop in `setup()`:

```cpp
MDNS.begin("bobby");   // now also reachable at http://bobby.local
```

`bobby.local` works on iPhones and Macs on any network. Many Android phones don't support it, so keep showing the IP on the OLED.

**Reconnect if Wi‑Fi drops** (router restart, weak signal). Change `loop()` to:

```cpp
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
    delay(1000);
    return;   // try again on the next loop
  }
  server.handleClient();
}
```

---

### In Wokwi

Your phone can't reach the simulator, but your **Mac's browser** can, through a **port forward** in `wokwi.toml`:

```toml
[wokwi]
version = 1
firmware = "build/10-web-remote.ino.bin"
elf = "build/10-web-remote.ino.elf"

[[net.forward]]
from = "localhost:8180"
to = "target:80"
```

With `Wokwi-GUEST` in `secrets.h`, start the simulator and open **`http://localhost:8180`** on your Mac. `localhost` means "this computer", and Wokwi passes port 8180 into the simulated ESP32's port 80.

### On the real board

1. Put your **real** Wi‑Fi details in `secrets.h`.
2. Upload and watch:
   ```bash
   source .env
   arduino-cli compile -u -p $PORT --fqbn $FQBN lessons/10-web-remote
   arduino-cli monitor -p $PORT -c baudrate=115200
   ```
3. Connect your phone to the **same Wi‑Fi**, and open the address shown on the OLED.

### About the IP address

Your router **lends** addresses to devices (this is called **DHCP**). The ESP32 usually gets the same one back, but it can change after the router restarts, and it's **different on every network**:

| Network | Typical ESP32 address |
|---|---|
| Home Wi‑Fi | `192.168.1.x` (depends on the router) |
| iPhone hotspot | `172.20.10.x` |
| Android hotspot | varies, often `192.168.43.x` |
| ESP32's own network (AP mode) | always `192.168.4.1` |

That's why step 1 shows the IP on the OLED. For a permanent address at home, look for **"DHCP reservation"** in your router's settings.

### If it doesn't work

| Problem | Fix |
|---|---|
| OLED stuck on "Connecting..." | Wrong Wi‑Fi details, 5 GHz network, or weak antenna (`setTxPower` line) |
| Phone says "can't reach this page" | Phone must be on the **same Wi‑Fi** (not mobile data or a guest network). Type **`http://`**, not `https://`. |
| Works on the Mac, not on the phone | Phone is on a different network, or the browser forced `https://` |
| Doesn't load on the Mac either | The router may block devices from talking to each other ("client isolation", common on guest networks) |
| Worked, then stopped | The IP changed. Press RST and read the OLED. |

**Safety:** anyone on the same Wi‑Fi can open this page and press the buttons. Fine at home; on shared networks, use your phone's hotspot. Don't expose it to the internet.

### Next

- **Lesson 11 ideas:** use it **away from home**: the ESP32 creates its own Wi‑Fi (`WiFi.softAP`, AP mode), joins your phone's hotspot (`WiFiMulti`), or lets you pick any network from your phone (the **WiFiManager** library), plus a login on the page (`server.authenticate`).
- **Try:** a brightness slider (`<input type="range" min="0" max="255" onchange="send('/bright?v=' + this.value)">` and a handler that calls `display.ssd1306_command(SSD1306_SETCONTRAST)` then `display.ssd1306_command(value)`), a button for the Lesson 8 clock, or one per Lesson 9 picture.

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
