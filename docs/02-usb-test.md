# Lesson 2: Arduino setup and first USB upload

## Objective
Verify that the computer can program the ESP32-WROOM-32 and receive messages from it. This test requires no LED wiring, Wi-Fi, or Telegram credentials.

## Prerequisites / setup recap
1. Install Arduino IDE from https://www.arduino.cc/en/software.
2. In File > Preferences, add https://espressif.github.io/arduino-esp32/package_esp32_index.json to Additional Boards Manager URLs, preserving any existing entries.
3. In Boards Manager, install esp32 by Espressif Systems.
4. Connect the board using a USB data cable.
5. Select ESP32 Dev Module for the typical WROOM-32 development board and select its port. The module name alone does not identify every development board variant.
6. Record the IDE version, installed ESP32 package version, board selection, and port in your lab record.

## Upload and observe
1. Open firmware/01_usb_test/01_usb_test.ino in Arduino IDE.
2. Confirm the board and port selections.
3. Click Verify (checkmark). Expected: compilation succeeds.
4. Click Upload (right arrow). This replaces the program currently on the board.
5. Wait for the upload to finish.
6. Open Tools > Serial Monitor and set the baud rate to 115200.
7. If desired, press the board EN/RESET button once to see the startup message again.

Expected output, with increasing numbers about every two seconds:

```text
Skillicon ESP32 USB test started
ESP32 is running | uptime seconds: 1
ESP32 is running | uptime seconds: 3
ESP32 is running | uptime seconds: 5
```

The initial message can be missed if Serial Monitor opens after setup runs. Repeated uptime messages are sufficient to confirm this test. A brief boot log is normal.

## Explain to students
setup() runs once after startup or reset. loop() repeats. Serial.begin(115200) sets the communication speed, which must match Serial Monitor. millis() measures elapsed milliseconds since startup; dividing by 1000 gives whole seconds. delay(2000) pauses this simple test for two seconds. Future networking code will need to remain responsive.

This test confirms programming and serial communication only; it does not verify Wi-Fi, Telegram, or OTA.

## Troubleshooting
- Stuck at Connecting: hold BOOT while the uploader connects; release when writing starts. Retry upload if it timed out. See Espressif troubleshooting below.
- Garbled output: set Serial Monitor to 115200 baud.
- No output: check the port, press EN/RESET, and wait a few seconds.
- Port missing: try a known USB data cable and another USB port. Identify your board USB-to-serial chip before installing a driver from its manufacturer.
- Port busy: close other serial terminal applications.
- Compilation failure: record the first meaningful error and confirm the ESP32 board package and board selection.

## Evidence and completion
Save a screenshot of successful upload and readable serial output, plus the actual versions and port used. Mark the test complete only after observing increasing uptime values on your board.

Status when authored: sketch prepared; compilation and hardware execution await the user's Arduino IDE test.

References:
- https://docs.espressif.com/projects/arduino-esp32/en/latest/guides/tools_menu.html
- https://docs.espressif.com/projects/arduino-esp32/en/latest/troubleshooting.html
