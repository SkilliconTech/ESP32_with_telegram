# ESP32 and Render workshop report

## From first USB connection to serial and web control

Prepared for SkilliconTech college workshops. Updated 20 November 2026.

This report explains how to connect an ESP32-WROOM-32 to a Wi-Fi router, publish a web dashboard on Render, and control an external LED from both Serial Monitor and a browser. It includes a beginner route without a personal GitHub account, the earlier Telegram experiment, troubleshooting, and a roadmap for advanced projects.

The instructor has confirmed working serial reporting and web LED control after the command-expiry fix. This is a single-device educational prototype. OTA updates, multiple-device routing, persistent history, and advanced controls are future extensions, not completed features.

Project source: https://github.com/SkilliconTech/ESP32_with_telegram

Instructor demonstration: https://esp32-with-telegram.onrender.com/

Students must use their own service URL and keys when setting up independent boards. Do not point the whole class at the instructor's device service.

## 1 What the system does

Serial Monitor is the text terminal in Arduino IDE. Type ON, OFF or BLINK and the ESP32 changes its LED locally. The ESP32 sends its mode, control source, uptime and Wi-Fi signal strength to Render. A browser displays the reports and lets the user issue the same three commands.

The communication paths are:

- Local control: Serial Monitor -> USB -> ESP32 -> GPIO23 -> LED.
- Reporting: ESP32 -> router -> HTTPS -> Render -> browser dashboard.
- Web control: browser -> Render pending command -> ESP32's next HTTPS request -> LED -> confirmation report -> browser.

The server does not directly connect through the home router to the ESP32. The board initiates outbound requests, so this design does not require router port forwarding. The browser can use a different internet connection from the board.

A GPIO is a controllable electrical pin. Firmware is the program uploaded to the ESP32. An API is an agreed set of requests and responses between programs. HTTPS protects those requests in transit. Deployment means installing and running the server program on Render.

## 2 Do students need GitHub or can they use Gmail

A Google account, often associated with a Gmail address, can be used to sign in to Render. Render also supports email/password and several Git provider login options. Login and code hosting are separate things: Gmail does not store and deploy the project source code. [1]

Students do not need personal GitHub accounts to deploy the instructor's public repository. They can choose Public Git Repository and paste its URL. The code remains hosted on the instructor's GitHub repository. No student GitHub write access or instructor password is needed. Services connected this way do not automatically deploy new commits; students use Manual Deploy when instructed. [2,3]

Recommended beginner route: one team, one ESP32, one Render service and two private keys. Sign in through Google, deploy the instructor's public repository, and download the code ZIP for Arduino IDE.

Optional development route: students create GitHub accounts, fork the project into their accounts, and connect those repositories to Render through Git Provider. This supports their own commits and automatic deployment when enabled. Google can still be their Render login method. [1,3]

For an instructor-only demonstration, viewers need neither a GitHub nor a Render account. They can watch the instructor's dashboard. Do not distribute the instructor's control key to a large audience unless shared control is intended.

## 3 Equipment and preparation

For each team prepare:

- One ESP32-WROOM-32 development board and a known USB data cable.
- One LED, one 330-ohm resistor, a breadboard and jumper wires.
- A computer with Arduino IDE and internet access.
- A router providing 2.4 GHz Wi-Fi with an ordinary network name and password.
- A Google/email account for Render, or an existing Render account.
- ArduinoJson version 7 for the final dashboard sketch.

A USB charging-only cable is insufficient. A board power light does not prove a data connection. Campus networks with web login portals or enterprise authentication need additional support; arrange a suitable workshop router with the college IT team beforehand.

Record the IDE version, ESP32 board-package version, ArduinoJson version and COM port for reproducibility. These vary between students and are not fixed by this report.

## 4 Obtain the source code

1. Open the public project repository above.
2. Choose Code > Download ZIP and extract it into a writable folder. A personal GitHub account is not needed to download public code.
3. Keep the folder structure intact. Arduino sketches are inside firmware, and the web application is inside server.
4. Open a sketch by its .ino file inside the matching folder.
5. For each lesson requiring credentials, copy secrets.example.h to secrets.h in that sketch's folder. Enter real values only in secrets.h.

Never put credentials into secrets.example.h. Git ignores secrets.h, but that protection does not remove secrets pasted into another tracked file. Compiled firmware may also contain credentials, so do not publish a configured board's binary.

## 5 Prepare Arduino IDE and the USB port

1. Install Arduino IDE from https://www.arduino.cc/en/software.
2. Open File > Preferences. Add the following under Additional Boards Manager URLs, preserving any existing entries:

```text
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

3. Open Boards Manager, search esp32, and install esp32 by Espressif Systems.
4. Connect the board by USB. For the typical WROOM-32 development board used here, select ESP32 Dev Module. WROOM-32 identifies the module; another development board may require a different selection.
5. Select Tools > Port and choose the port that appears when this board is connected.

### If the COM port is missing

Open Windows Device Manager. Observe the device list while disconnecting and reconnecting the board. In this project, CP2102 USB to UART Bridge Controller appeared under Other devices with a yellow warning, and Properties reported Code 28: the driver was missing. An unrelated COM8 remained present, so it was not the board's port.

For a CP2102 board, download the CP210x Universal Windows Driver from the official Silicon Labs page [5]. Extract the ZIP. In Device Manager, choose Update driver > Browse my computer for drivers, select the extracted directory, enable Include subfolders and proceed. Reconnect the board, then select the new CP210x COM port in Arduino IDE. Boards using another USB bridge require their own manufacturer's driver.

## 6 Verify USB programming before networking

1. Open firmware/01_usb_test/01_usb_test.ino.
2. Click Verify, then Upload. Uploading replaces the program on the board.
3. Open Serial Monitor at 115200 baud.
4. Confirm that increasing uptime values appear about every two seconds.

If upload stalls at Connecting, hold BOOT while the uploader connects and release it once writing begins. If text is unreadable, check the baud rate. A brief unreadable fragment followed by correct messages does not by itself indicate a failure.

Checkpoint: successful upload and readable recurring output. Save a screenshot for the lab report.

## 7 Verify Wi-Fi separately

1. Open firmware/02_wifi_test/02_wifi_test.ino.
2. Create secrets.h from the example if needed. Enter the Wi-Fi network name and password, retaining the C++ quotation marks.
3. Upload and open Serial Monitor at 115200 baud.
4. Wait for Wi-Fi connected, a local IP address and signal strength.

Use a 2.4 GHz-capable network. A combined 2.4/5 GHz name can work; a 5 GHz-only network cannot serve this board. Names and passwords are case-sensitive. Escape embedded quotation marks or backslashes in C++ string values.

This checkpoint confirms connection to the router. It does not alone prove access to Telegram or Render.

## 8 Wire the external LED

Disconnect USB power before wiring or making corrections.

GPIO23 -> 330-ohm resistor -> LED anode, normally the longer leg.

LED cathode, normally the shorter leg and flat package side -> GND.

Use the pin labelled 23 or IO23, not the twenty-third physical pin. The resistor must remain in series with the LED. Reconnect USB after checking the circuit. Do not connect motors, pumps or mains loads directly to a GPIO; those require suitable drivers and a separate electrical design.

## 9 Optional earlier Telegram lesson

Telegram is not required for the final serial/web dashboard. We used it as an earlier lesson in remote messaging.

Create a bot with the official @BotFather using /newbot. Choose a unique username ending in bot and store its token privately. Add start, help, status, led_on and led_off through /setcommands. This configures a menu, not the program logic.

Install UniversalTelegramBot by Brian Lough and ArduinoJson through Library Manager. Follow docs/04-telegram-test.md to discover the private chat ID locally, set ALLOWED_CHAT_ID and verify a /start reply. Then use firmware/04_telegram_led to test /led_on and /led_off with GPIO23.

Each team should use its own bot for that optional exercise. Uploading the final dashboard sketch replaces the Telegram sketch: Telegram LED commands will no longer be processed by that board. Bot tokens are not needed in Render for the final dashboard.

## 10 Create a Render service from the beginning

### Route A for students without GitHub accounts

1. Visit https://dashboard.render.com/ and create or sign in to an account using Google or email/password.
2. Choose New > Web Service, or Deploy a Web Service on an empty dashboard.
3. Select Public Git Repository rather than Git Provider.
4. Paste https://github.com/SkilliconTech/ESP32_with_telegram.git and choose Connect.

### Route B for students maintaining their own code

1. Sign in to Render and choose New > Web Service.
2. Select Git Provider, connect GitHub and authorize access to the student's fork.
3. Select that repository. Use the instructor repository directly only where access and the workshop plan permit it.

### Settings for both routes

- Name: a unique team name, such as esp32-team07-lab.
- Language/runtime: Python 3.
- Branch: main.
- Region: select an offered region; the instructor service used Oregon.
- Root Directory: server.
- Build Command: pip install -r requirements.txt.
- Start Command: the command below.
- Compute/instance type: Free.
- Health Check Path: /health.

```text
gunicorn --workers 1 --threads 4 --bind 0.0.0.0:$PORT app:app
```

Copy the command exactly. The dollar sign in $PORT is required; do not add an extra shell prompt character at the beginning. One worker is mandatory for the current in-memory state design. Multiple workers would each hold separate device state.

Render runs the server, installs dependencies and supplies its network port and public HTTPS address. The settings above are specific to this project's Flask application. [2]

## 11 Add private keys and deploy

In the service's Environment section, add DEVICE_KEY and DASHBOARD_KEY. Use Generate for each so they contain different long random values. Save them securely without quotation marks in Render.

DEVICE_KEY authenticates the board's requests. DASHBOARD_KEY gives control access through the webpage. Neither key is a Telegram token. Avoid exposing them in screenshots or classroom screen sharing. Anyone with the dashboard key can issue commands to that service.

Choose Deploy Web Service, or save and deploy if the service already exists. Wait for Live. Open the assigned URL and then its /health path. The expected health response is {"status":"ok"}. The homepage should show ESP32 control lab.

For the public-repository route, use Manual Deploy > Deploy latest commit after the instructor publishes a fix. For connected Git-provider deployments, automatic updates depend on the service's deployment settings. [3]

A render.yaml file is also in the repository for future Blueprint use. Merely committing it does not change settings on a manually configured existing service.

## 12 Configure and upload the final ESP32 sketch

1. Open firmware/05_render_dashboard/05_render_dashboard.ino.
2. Ensure ArduinoJson 7 is installed.
3. Create secrets.h from secrets.example.h if missing.
4. Fill in WIFI_SSID, WIFI_PASSWORD, SERVER_URL and DEVICE_KEY. Use this team's Render URL without a trailing slash and the exact matching device key.

```cpp
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* SERVER_URL = "https://YOUR-SERVICE.onrender.com";
const char* DEVICE_KEY = "YOUR_PRIVATE_DEVICE_KEY";
```

5. Leave server_roots.h beside the sketch; it contains public certificate authorities used for HTTPS verification.
6. Verify and upload using the board's actual COM port.
7. Open Serial Monitor at 115200 baud and select Newline or Both NL and CR.
8. Send BLINK, then ON, OFF and STATUS. Local control should work even while the network is unavailable.

The sketch synchronizes its clock for certificate checking. Do not fix connection errors by disabling HTTPS certificate verification. This program has been compiled successfully for ESP32 Dev Module, but each team's upload and physical tests are still required.

## 13 Connect the webpage and prove both directions

Open the team's service URL. Enter DASHBOARD_KEY and click Connect. The key stays only in that tab's memory; a reload clears it. The input clearing after connection is expected.

Acceptance sequence:

1. Send BLINK through Serial Monitor. Observe actual blinking and BLINK / SERIAL on the dashboard.
2. Click Turn off. Observe Pending first, then OFF / WEB, device confirmation and the physical LED switching off.
3. Click Turn on. Confirm the LED lights and the reported source is WEB.
4. Send OFF through Serial Monitor. Confirm the page changes to OFF / SERIAL.
5. Disconnect the board. After approximately 20 seconds without a report, the page should show offline and disable buttons.
6. Reconnect the board and confirm status reporting resumes.

Keep expected and actual results separate in the student report. A server health response only proves the server can respond. A device acknowledgment only proves firmware handled the instruction; the physical LED must also be observed.

## 14 How the software works

server/app.py defines the dashboard and APIs. server/templates/index.html provides the controls and refreshes status. The final .ino handles local input, GPIO23 and network synchronization.

GET /health is a public server health check. GET /api/status and POST /api/command require the dashboard key. POST /api/device/sync requires the device key and carries the latest telemetry plus the previous command's acknowledgment.

The board reports roughly every two seconds while requests succeed; the browser refreshes roughly every three seconds. Network latency and cold starts increase response time, so this is not real-time industrial control.

BLINK toggles the output every 500 milliseconds. Network activity runs in a separate task, allowing serial input and blinking to continue during a slow request. Shared state is protected by a mutex so concurrent tasks do not edit it unsafely.

Only the latest pending web command is retained. It expires after 30 seconds. A new serial command received while a web response is in flight supersedes that response. Otherwise, commands take effect in the order applied by the board. Reports describe the commanded mode, not a measured optical LED state.

## 15 Troubleshooting and the fix discovered in this project

Missing port: check cable/data capability, USB socket and Device Manager. Code 28 on the CP2102 means its driver is missing; install the correct official driver as described above.

Device offline: confirm the final sketch is uploaded, router internet works, the URL is correct and DEVICE_KEY matches. Inspect Serial Monitor. HTTPS also needs a valid clock.

HTTP 401 or incorrect key: the ESP32 uses DEVICE_KEY; the browser uses DASHBOARD_KEY. Check for copied quotation marks or extra spaces.

HTTP 403 or a browser-verification challenge: the hosting edge may be blocking automated requests. The ESP32 cannot solve a JavaScript challenge. Investigate the hosting/network behavior rather than disabling certificate checks.

HTTP -1 or time synchronization failures: investigate Wi-Fi, DNS, internet/NTP access, the service URL and certificate trust. Retry after a cold start.

Bot replies but dashboard does not update: the old Telegram sketch may still be running. Upload the dashboard sketch and use its own settings file.

Reported ON but LED dark: disconnect USB before checking LED polarity, resistor placement, breadboard rows, GPIO23 and GND.

### Command expiry bug and regression test

The initial server sent expiry as a decimal number. Firmware used ArduinoJson's integer default-value operator, which could reject that type and produce zero. The board then treated the fresh command as expired and returned a misleading superseded result.

The server was corrected to emit whole-number epoch seconds. Commit 54d80b2 contains the fix, and a regression test checks both command creation and delivery. The user confirmed web control worked after deployment. Students should use that commit or a later version. No firmware re-upload was required for this server-side fix.

## 16 Add buttons and sensor data

A functional control needs changes across the whole path, not only a new HTML button.

For adjustable blink speed, first define a command with a bounded interval, for example 100 to 5000 milliseconds. Add an input and submit action on the webpage. Extend server validation and command delivery. Update firmware to validate and apply the interval, replace the hard-coded 500 ms timing, and include the applied interval in reports. Test invalid values, offline behavior and confirmation. This is a proposed exercise, not functionality in the current code.

For an independent buzzer or second LED, expand the command model beyond a single mode. Include a target channel and action, track each output separately, and report each state. Simply adding BUZZER_ON to the existing LED mode enum is insufficient because the existing telemetry and firmware assume ON, OFF or BLINK for one LED.

For sensor data, choose a supported sensor, wire it correctly, sample it on the board, add bounded telemetry fields to the server and display them on the page. Persistent charts require a database rather than this server's temporary memory.

Server/UI changes deploy through Render. Firmware changes require USB upload until OTA is implemented.

## 17 Advanced roadmap and current limits

Multiple boards: introduce a unique device ID and separate credentials, state and command routing for each device. Add a device selector and access rules. Connecting many boards to the current service will mix reports and commands.

Persistent history: use an external database with device/time indexing and a retention policy. Replace in-memory commands with durable storage before running multiple workers or instances.

OTA: design authenticated firmware releases, hardware/version checks, integrity verification, compatible OTA partitions and recovery/rollback. Never distribute firmware binaries containing classroom Wi-Fi credentials. OTA remains unimplemented.

Stronger access control: replace shared dashboard keys with user accounts and device-level permissions if the project grows beyond a small workshop. Add request limiting, audit records and key rotation.

Render's Free tier is useful for learning, but idle services can sleep after 15 minutes and may take about a minute to wake. Runtime files are temporary. Free instance hours and bandwidth/build quotas apply; multiple services in the same workspace share relevant allowances. Confirm current limits before scheduling a class. [4]

The current service loses memory state on restart or deployment. The next board report repopulates status; pending commands may need resending. This is not a safety-critical controller.

## 18 Instructor plan and student report checklist

Before class: preflight account registration and free-plan availability, download a known-working code version and official drivers, confirm campus network suitability, and test a complete team setup. Prepare spare USB data cables. Do not share personal Google/GitHub passwords.

Suggested 2-hour teaching plan: 15 minutes for concepts and account setup; 20 for IDE/USB verification; 15 for wiring and Wi-Fi; 25 for Render deployment and keys; 25 for serial/web acceptance tests; 20 for troubleshooting and extension discussion. This is a planning estimate; installation and network delays can require additional time. Keep Telegram as an optional separate lesson.

Each team's submission should include the objective, circuit description, software versions, public service URL, selected deployment route, numbered actions, expected/actual results, sanitized evidence, faults and fixes, limitations and one proposed extension. Never include tokens, keys or Wi-Fi passwords.

Final presentation: show BLINK from Serial Monitor, show SERIAL on the webpage, turn the LED off from the web, show WEB and acknowledgment, then explain that the ESP32 initiates the internet requests. Explain which capabilities were demonstrated and which remain future work.

## 19 References and project files

Official platform references checked 20 November 2026:

[1] Render login methods: https://render.com/docs/login-settings

[2] Render web services and public-repository deployment: https://render.com/docs/web-services

[3] Render deployment triggers and manual deploys: https://render.com/docs/deploys

[4] Render free-service limits: https://render.com/docs/free

[5] Silicon Labs CP210x drivers: https://www.silabs.com/software-and-tools/usb-to-uart-bridge-vcp-drivers

[6] Espressif Arduino setup: https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html

[7] Telegram bot tutorial for the optional lesson: https://core.telegram.org/bots/tutorial

Project lesson files: docs/01-telegram-setup.md through docs/07-render-dashboard.md. Evidence template: docs/lab-record-template.md. Development history and reported outcomes: docs/progress-log.md.
