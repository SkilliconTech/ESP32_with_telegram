# ESP32-WROOM-32 with Telegram

A step-by-step college workshop by SkilliconTech.

Start with the [complete beginner-to-advanced workshop report](docs/08-complete-workshop-report.md), including the Google sign-in route for students without GitHub accounts.

## Learning path
1. [Create and configure the Telegram bot](docs/01-telegram-setup.md).
2. [Prepare the ESP32-WROOM-32 and verify a USB upload](docs/02-usb-test.md).
3. [Connect to the Wi-Fi router](docs/03-wifi-test.md), then [test Telegram commands](docs/04-telegram-test.md) and [control an LED](docs/05-telegram-led.md).
4. [Deploy Render](docs/06-render-hosting.md), then [use the Serial + Web LED dashboard](docs/07-render-dashboard.md).
5. Add authenticated firmware updates over Wi-Fi (planned).

Current stage: Serial Monitor controls the ESP32 locally; ESP32 HTTPS reports go to Render, and a web dashboard sends commands back. Earlier lessons demonstrate direct Telegram control. Telegram is not used by the new dashboard sketch. OTA remains a later stage.

Remote control sends instructions to an existing program. OTA replaces the program running on the board. They are separate milestones.

## Current status
User reported Telegram bot creation, Arduino IDE board setup, and successful USB serial test. Router Wi-Fi connection and Telegram library installation are user-confirmed. The first Telegram reply is user-confirmed. Physical LED control is user-confirmed. Render health is user-confirmed. Lesson 7 adds serial/web control and awaits the new firmware upload and end-to-end hardware verification. OTA remains unimplemented.

## Equipment for later lessons
- ESP32-WROOM-32 development board, USB data cable, computer.
- Internet-connected 2.4 GHz Wi-Fi and a Telegram account.
- Arduino IDE is the proposed teaching environment; confirm before the firmware lesson.
- An external LED and resistor may be needed depending on the development board.

## Documentation practice
For each lesson record the objective, actions, expected result, actual result, and troubleshooting. Use [the lab record template](docs/lab-record-template.md). Keep screenshots under docs/images after removing private information. Record verified milestones in [the progress log](docs/progress-log.md).

## Credentials
This repository is public. Never commit bot tokens, Wi-Fi passwords, device keys, or real environment files. The ignore file excludes common secret files but does not detect secrets embedded in documentation or code. Review files before pushing. Replace an exposed Telegram token using BotFather.

## Hosting note
Render free services can sleep after 15 minutes without inbound traffic and may take about a minute to wake. Later lessons must handle delays and retries. Do not rely on the free service filesystem to permanently store firmware uploads or pending commands.

## References
- https://core.telegram.org/bots/tutorial
- https://core.telegram.org/bots/features
- https://core.telegram.org/bots/api
- https://render.com/docs/free
- https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/ota.html
