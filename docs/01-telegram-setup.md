# Lesson 1: Create your Telegram bot

## Objective
Create a bot identity and a command menu for the ESP32 workshop. No board or server is required yet. Allow approximately 10-15 minutes.

## 1. Open the official BotFather
Open https://t.me/BotFather in Telegram. Check that the username is exactly @BotFather. Tap Start if prompted.

Why: BotFather registers and configures bots. It does not run the code that controls your ESP32.

Expected result: BotFather displays its available commands.

## 2. Create the bot
Send /newbot. When asked for a display name, enter Skillicon ESP32 Lab (or your preferred name). When asked for a username, choose a unique username ending in bot, such as SkilliconESP32Lab_bot. This example may already be taken; add a unique suffix before bot if necessary.

Expected result: BotFather returns your bot link and an API token.

## 3. Save the token privately
Save the token in a password manager or another private location. Treat it as the bot password. Do not paste it into chat, GitHub, slides, screenshots, or browser URLs shown during a presentation. Later we will enter it locally or into server environment settings.

If exposed, revoke/replace it through BotFather before continuing. Do not keep using the exposed token.

Documentation: record only the bot display name, username, creation date, and that the token was stored privately. Never record the token in the lab report.

## 4. Set the description
Send /setdescription to BotFather and select your bot. Enter:

College IoT workshop: control an ESP32-WROOM-32 and view its status using Telegram.

Expected result: BotFather confirms the description update.

## 5. Set the command menu
Send /setcommands to BotFather and select your bot. Send the following lines as one message, without leading slashes:

```text
start - Show the welcome message
help - List available commands
status - Show ESP32 connection status
led_on - Turn the demonstration LED on
led_off - Turn the demonstration LED off
```

Expected result: BotFather confirms that the command list was updated.

These entries are menu labels only. They do not implement any behavior. We will add /update only when OTA is implemented and tested.

## 6. Open your own bot
Open the bot link supplied by BotFather and tap Start or send /start. Check the command menu; reopen the chat if it has not refreshed.

Expected result: your /start message appears. No automatic reply is expected yet because we have not connected a running program. Creating a bot is not the same as running one.

## 7. Capture classroom evidence
Capture the bot profile, command menu, and /start message. Do not capture the BotFather message containing the token. Inspect every screenshot before saving or publishing it. Record the actual result using lab-record-template.md.

## Troubleshooting
- Username rejected: choose a different unique username ending in bot.
- Commands rejected: use lowercase command names, one per line, followed by a space, hyphen, and description; omit the leading slash.
- No reply to /start: expected at this stage.
- Menu missing: confirm you selected the correct bot in BotFather, then reopen its chat.

## Explain this to students
A Telegram bot has an identity, a private API credential, and program logic. Today we created its identity and menu. Later our program will receive messages, check who sent them, and communicate with the board. A bot username is public; its token is private.

## Completion checkpoint
- [ ] Bot created.
- [ ] Token saved privately.
- [ ] Description configured.
- [ ] Five commands visible.
- [ ] /start sent to the new bot.
- [ ] Sanitized evidence recorded.

Next: share only the bot username and confirm whether Arduino IDE is installed. Do not share the token.

Official references, checked 12 November 2026:
- https://core.telegram.org/bots/tutorial
- https://core.telegram.org/bots/features
