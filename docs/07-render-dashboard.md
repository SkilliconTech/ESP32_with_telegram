# Lesson 7: Serial control and Render web dashboard

## Goal and current architecture
The ESP32 accepts ON, OFF, BLINK and STATUS from Serial Monitor. It reports to the Render server over authenticated HTTPS every roughly two seconds while connected. The browser displays these reports and submits web commands; the ESP32 fetches and acknowledges them. Telegram is not used in this stage. Keep GPIO23 -> 330-ohm resistor -> LED anode; LED cathode -> GND.

## 1. Configure the existing Render service
After the GitHub update deploys, the homepage becomes a dashboard.
1. Open Render > your existing service > Environment.
2. Add DEVICE_KEY and DASHBOARD_KEY. Use Generate for each, producing two different long random values. Keep them private. DEVICE_KEY goes on the ESP32; DASHBOARD_KEY unlocks the webpage.
3. Save the environment settings. Old TELEGRAM_BOT_TOKEN and ALLOWED_CHAT_ID variables are unused by this version and can be removed.
4. Under Settings, set Start Command exactly to:
   gunicorn --workers 1 --threads 4 --bind 0.0.0.0:$PORT app:app
5. Keep Root Directory server, Python 3, build command pip install -r requirements.txt, Free plan, and health check /health.
6. Wait until the latest deployment is Live. If automatic deployment is disabled, use Manual Deploy > Deploy latest commit.

render.yaml documents these settings for future Blueprint deployments. Adding this file does not reconfigure an existing manually created service; complete the dashboard steps above.

## 2. Upload the new sketch
1. Open firmware/05_render_dashboard/05_render_dashboard.ino.
2. Use ArduinoJson 7 (the sketch uses JsonDocument); update via Library Manager if needed.
3. In secrets.h, fill WIFI_SSID, WIFI_PASSWORD, SERVER_URL and DEVICE_KEY. The example URL is https://esp32-with-telegram.onrender.com without a trailing slash. Copy the DEVICE_KEY from Render exactly, keeping C++ quotation marks. A new private secrets.h is provided locally; fresh clones must copy secrets.example.h first.
4. The DASHBOARD_KEY and Telegram bot token do not belong in this sketch.
5. Verify and upload to ESP32 Dev Module using the existing COM port.
6. Open Serial Monitor at 115200 baud. Choose Newline (or Both NL & CR).
7. Send ON, OFF and BLINK. The LED should switch immediately. BLINK changes GPIO level every 500 ms. STATUS prints the current mode and control source.

## 3. Open the web dashboard
1. Open https://esp32-with-telegram.onrender.com/.
2. Enter DASHBOARD_KEY and click Connect. The key is held only in browser memory; reload clears it.
3. Once the sketch reports, Device online and its reported mode/source/uptime appear.
4. Type BLINK in Serial Monitor. Expect the LED to blink and the dashboard to show BLINK / SERIAL within a few seconds.
5. Click Turn off on the web. The page first shows Pending; after receipt and the next device report it shows confirmation and OFF / WEB.
6. Test Turn on and Blink, then send OFF in Serial Monitor.
7. Disconnect the board: after 20 seconds without reports the dashboard marks it offline and disables web commands. Reconnect and wait for reporting to resume.

## Ordering and limits
- Commands take effect in the order applied on the ESP32. A serial command entered while a web response is in flight takes priority over that response.
- Only the most recent pending web command is retained. Commands expire after 30 seconds; offline commands are rejected.
- Repeated delivery of the same command ID does not reapply it during the same device boot.
- The dashboard reports the GPIO mode commanded by firmware; it has no optical sensor proving the LED is physically lit. It does not animate every blink.
- A network task performs HTTPS separately so slow network operations do not block serial input or the blink timer.
- This is a single-device workshop service. State and pending commands live in one server process, so exactly one Gunicorn worker is required. Restarts and redeploys clear them. The ESP32's next report restores displayed state; an unconfirmed command may need resending. This is not durable telemetry storage or multi-device production control.
- Free Render services can sleep and have quotas. Leave time for a cold start; the ESP32 retries after errors. Reports while powered on count as inbound traffic.
- The included public Google and ISRG trust roots enable HTTPS certificate verification. Never fix a certificate error by disabling verification; inspect the current service certificate chain and update trust roots if needed.
- This firmware replaces direct Telegram polling. It does not implement Telegram or OTA.

## Troubleshooting
- Unauthorized / HTTP 401: compare the appropriate key privately. Browser uses DASHBOARD_KEY, device uses DEVICE_KEY.
- HTTP 403 or a browser-challenge page: the hosting edge may be challenging automated requests. An ESP32 cannot solve JavaScript challenges; investigate Render/network filtering before continuing. Do not disable TLS checks.
- HTTP -1: check URL, Wi-Fi, time sync and certificate trust. Retry after the service wakes.
- Offline: ensure the new sketch is uploaded and sends reports. Verify internet/NTP access and Serial Monitor diagnostics.
- Settings seem to disappear: confirm one worker and note restart behavior above.
- Serial commands do nothing: select Newline and check GPIO23 wiring.

## Evidence for the college demonstration
Record: local ON/OFF/BLINK response; dashboard mode/source after a serial command; web Pending then device confirmation; offline indication after disconnect. Capture a dashboard screenshot without keys. Record actual hardware results separately from automated API tests.
