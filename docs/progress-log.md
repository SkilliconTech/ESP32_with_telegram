# Progress log

## 12 November 2026
- Selected ESP32-WROOM-32 for the first version; ESP32-C3 is deferred.
- Created workshop overview, Telegram lesson, and lab record template.
- Local project: D:\Projects\CAN DataLogger\Single Channel\SkillLogger\ESP32_Telegram
- GitHub: https://github.com/SkilliconTech/ESP32_with_telegram
- Pending user verification: bot creation and command menu.
- No firmware uploaded, Telegram replies tested, Render service deployed, or OTA performed yet.

## 13 November 2026
- User confirmed bot username matches the suggested SkilliconESP32Lab_bot and Arduino IDE is installed.
- User reported completion of ESP32 board-package installation and board/port selection instructions. Exact software versions and port are not recorded yet.
- Added lesson 2 and a USB serial test sketch. Hardware upload and output verification remain pending.
- Previous GitHub push was denied for the locally authenticated account AnuragShirodkar; remote publication remains unconfirmed.
- User verified the USB test: readable uptime values 63, 65, 67, 69, and 71 seconds. Lesson 2 passed based on reported serial output.
- Selected a Wi-Fi router for the next test.
- Added lesson 3, a Wi-Fi connection sketch, and a public credential template. Actual Wi-Fi connection remains pending.
- User confirmed router Wi-Fi connection and installation of UniversalTelegramBot and ArduinoJson (versions pending).
- Added lesson 4: direct Telegram HTTPS polling, local chat ID discovery, and private-chat authorization. Telegram exchange awaits hardware verification.
- Preserved the edited Wi-Fi public template in a Git-ignored local backup and restored public placeholders to avoid publishing possible credentials.
- User confirmed receipt of the first Telegram reply; two-way Telegram communication passed. Separate /status confirmation has not yet been reported.
- User has an external LED and 330-ohm resistor and reports completing the GPIO23 wiring.
- Added lesson 5 and a separate LED-control sketch; physical on/off testing remains pending.
- User confirmed physical LED on/off control through Telegram. Lesson 5 passed based on the user's report.
- All five original project commits were pushed individually to GitHub after collaborator access was fixed.
- Added lesson 6: a Render hosting checkpoint with / and /health endpoints. Public deployment, server-mediated Telegram commands and OTA remain pending.

## 19 November 2026
- User verified the deployed Render /health response at https://esp32-with-telegram.onrender.com/health.
- Scope clarified: serial and web LED control with status sent to Render; Telegram is not part of this stage.
- Added authenticated dashboard and single-device synchronization API, with command expiry, acknowledgment and offline detection.
- Added separate ESP32 sketch with nonblocking blink/serial loop and a network task; hardware upload and end-to-end verification remain pending.
- Documented Render keys, single-worker requirement, temporary server state, setup and demonstration checklist in lesson 7.
- Validation: five server API tests passed; dashboard JavaScript syntax checked; ESP32 Dev Module firmware compiled successfully (1,065,668 bytes flash; 48,920 bytes globals). Browser visual QA and physical end-to-end testing remain pending.

## 20 November 2026: Web-command expiry fix
- User uploaded the dashboard sketch. Screenshot confirmed online telemetry, BLINK mode and SERIAL source, but web commands were reported as superseded.
- Found protocol mismatch: server emitted fractional expiry seconds while firmware reads an integer with ArduinoJson's default-value operator. Changed the server to emit integer epoch seconds.
- Added a regression test checking integer expiry in both command creation and device delivery. All six server tests pass.
- Fix is server-only; no firmware re-upload needed. Await Render deployment of 54d80b2 or later and a fresh physical web-command test.
