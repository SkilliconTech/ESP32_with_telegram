# Lesson 4: First Telegram exchange

## Objective
Receive /start and /status directly on the ESP32 and reply to the owner's private chat. Render, LEDs and OTA are later lessons. Each team uses its own bot and board.

## Prerequisites
USB and router Wi-Fi tests passed. Install UniversalTelegramBot by Brian Lough and ArduinoJson by Benoit Blanchon through Library Manager. Record the installed versions in the lab report.

## Part A: Discover your private chat ID
1. Open firmware/03_telegram_test/03_telegram_test.ino.
2. Open secrets.h. For a fresh GitHub clone, copy secrets.example.h to secrets.h in this sketch folder first.
3. Fill WIFI_SSID, WIFI_PASSWORD and BOT_TOKEN in secrets.h only. Use the token issued by BotFather. Keep quotation marks; escape embedded quotes and backslashes in C++ strings.
4. Leave ALLOWED_CHAT_ID empty. Save and upload with your working ESP32 board/port settings.
5. Open Serial Monitor at 115200 baud. Wait for Telegram connected. Wi-Fi and clock synchronization can each take up to 30 seconds.
6. Open your own bot in a private chat and send /start now.
7. Serial Monitor prints Private chat ID followed by a number. No Telegram reply is expected during discovery. Use the ID from your own freshly sent message, not another person's message.

## Part B: Authorize your account
1. Copy that number into ALLOWED_CHAT_ID in secrets.h, inside quotation marks.
2. Save and upload again.
3. After Telegram connected appears, send /start again. Expected reply: Hello from your ESP32-WROOM-32! Telegram communication is working.
4. Send /status. Expected: ESP32 online and uptime seconds.
5. /help lists the currently supported commands. The BotFather LED menu entries do not control hardware yet.

## How it works
The ESP32 polls Telegram over HTTPS. Telegram updates contain the sender, chat ID and message text. Discovery displays IDs locally without replying; configured mode only responds when both the private chat and sender match the configured ID. A trusted root certificate and network-synchronized clock validate HTTPS. Do not replace certificate checking with setInsecure().

Only one program should poll a given bot. When migrating to Render, stop this direct polling sketch before enabling the server webhook: getUpdates cannot be used while a webhook is configured.

## Troubleshooting
- secrets.h missing: copy the example to that exact filename beside the sketch.
- Compilation error: record the first meaningful error and both library versions. Do not share a full log if it includes credentials.
- Wi-Fi failed: check credentials and 2.4 GHz connectivity, then reset.
- Time sync failed: check router internet access and NTP restrictions, then reset.
- Telegram connection failed: check the token locally and internet access; check the installed library's root certificate if necessary. Do not disable HTTPS verification.
- Telegram connected but no updates: send a new private message to the correct bot; stop other bot polling programs and check whether an existing webhook was configured. Do not delete a webhook for another active deployment without checking it.
- No reply during discovery: expected. Configure your own chat ID and upload again.
- No reply after configuration: verify the ID locally, send /start again, and inspect Serial Monitor.

## Evidence and status
Record the successful /start reply and /status response with sanitized screenshots. Keep secrets.h and credential-bearing compiled firmware private. The sketch is prepared but has not been compiled or hardware-tested by the assistant; the user's upload and observed replies are the verification checkpoint.

References:
- https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
- https://core.telegram.org/bots/api#getupdates
