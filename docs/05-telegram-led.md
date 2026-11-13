# Lesson 5: Control an external LED through Telegram

## Objective
Use an authorized private Telegram chat to switch GPIO23 and observe a physical LED. This is direct ESP32-to-Telegram communication; Render and OTA are not involved yet.

## Wiring
Disconnect USB power before changing wiring.
- GPIO23 (labelled 23 or IO23) -> 330 ohm resistor -> LED anode (long leg).
- LED cathode (short leg, usually flat side of package) -> GND.
Keep the resistor in series. Reconnect USB after checking the circuit.

## Upload
1. Open firmware/04_telegram_led/04_telegram_led.ino in Arduino IDE.
2. The existing local secrets.h was copied from lesson 4 without displaying its contents. Check privately that it has the Wi-Fi details, bot token, and your authorized chat ID. For a fresh clone, copy secrets.example.h to secrets.h and fill these in locally.
3. Select the same ESP32 board and port. Verify and upload.
4. Open Serial Monitor at 115200 baud. Wait for Telegram connected.

## Test
1. Send /led_off. Expected: reply LED output set to OFF and the physical LED is dark.
2. Send /led_on. Expected: reply LED output set to ON and the LED lights.
3. Send /status. Expected: uptime and LED output: ON.
4. Send /led_off. Expected: LED goes dark.
5. Repeat the on/off cycle and record actual results.

/status reports the commanded GPIO state, not a sensor measurement of the LED. A reply alone cannot confirm correct wiring. The output is initialized LOW at startup; previously queued Telegram commands may subsequently change it. This teaching sketch is intended for an LED, not machinery or mains-powered loads.

## Explain to students
pinMode configures GPIO23 as an output. digitalWrite HIGH drives the LED circuit; LOW switches it off. The resistor limits current. The program checks the private chat and sender before handling commands. The saved ledOn variable reports the requested state.

## Troubleshooting
- Bot replies but LED stays dark: unplug USB, check polarity, GPIO23, GND, resistor and breadboard row connections. Reconnect and retry.
- LED always on: check that it is connected to GPIO23, not the 3.3 V supply pin.
- No bot reply: check Serial Monitor, the authorized ID, internet connection, and the lesson 4 troubleshooting guide.
- Missing secrets.h: copy the placeholder example to that filename and fill credentials locally.

## Workshop record
Capture /led_on and /led_off replies and photos of the corresponding physical LED state. Record the resistor value (330 ohms), GPIO23 and actual outcomes. Do not show credentials. Compilation and hardware verification of this new sketch await the user's IDE upload.
