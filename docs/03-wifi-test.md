# Lesson 3: Connect to your Wi-Fi router

## Objective
Join the router's 2.4 GHz network and obtain an IP address. This lesson verifies local Wi-Fi connectivity, not internet access or Telegram communication.

## Steps
1. Ensure your router has 2.4 GHz Wi-Fi enabled. A combined 2.4/5 GHz network name can work; a 5 GHz-only network cannot serve this board.
2. Open firmware/02_wifi_test/02_wifi_test.ino in Arduino IDE.
3. Open the secrets.h tab. A placeholder copy is provided locally. Students cloning GitHub must copy secrets.example.h to secrets.h in the same sketch folder, then reopen the sketch.
4. In secrets.h only, replace YOUR_WIFI_NAME and YOUR_WIFI_PASSWORD with your router credentials, keeping the double quotes. Names and passwords are case-sensitive. In C++ strings, write a literal backslash as \\ and a literal double quote as \".
5. Save, verify, and upload using the same board and COM port as lesson 2.
6. Open Serial Monitor at 115200 baud. Allow up to 30 seconds for the initial connection.
7. Look for Wi-Fi connected, a local IP address, and signal strength. Connected status is printed every ten seconds.

Example output (your address and signal will differ):

```text
Wi-Fi connected | IP: 192.168.1.42 | Signal (dBm): -55
```

## Explain to students
WiFi.h comes with the Espressif board package; no extra library is needed. Station mode makes the ESP32 a client of the router. The router normally assigns a local address using DHCP. RSSI describes received signal strength; a less negative number generally indicates a stronger signal. Internet access will be checked in the Telegram lesson.

## Troubleshooting
- Repeated retries: check the exact network name and password locally, 2.4 GHz availability, and distance from the router.
- Missing secrets.h: copy secrets.example.h to secrets.h in this sketch folder.
- Compilation errors after entering credentials: check quotation marks and escape backslashes or embedded double quotes.
- College Wi-Fi with a login webpage or enterprise username: this simple personal-network example is insufficient. Use the agreed workshop router or obtain campus IT guidance.
- Unexpected resets: check USB power and cable quality.

## Private information
secrets.h is ignored by Git. Never edit real credentials into secrets.example.h or the sketch. Do not show the secrets tab during screen sharing. Compiled firmware can contain credentials, so do not publish credential-bearing binaries. Review screenshots before publishing.

## Lab evidence
Record board/package versions and whether the connection succeeded. Capture serial output without revealing credentials. Optionally redact your local IP in classroom materials. Do not record the router password. Hardware verification is pending until the user reports the output.

Reference: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
