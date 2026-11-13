# Lesson 6: Deploy the first server on Render

## Objective
Publish a Python web service and verify its public HTTPS address. This checkpoint does not handle Telegram commands or communicate with the board yet. Keep lesson 5 firmware running; no bot token is needed for this deployment.

## Create the service
1. Open https://dashboard.render.com/ and sign in or create an account.
2. Select New > Web Service.
3. Connect GitHub and choose SkilliconTech/ESP32_with_telegram. Grant access to this repository when asked. If it is not listed, check which GitHub account is connected and its repository access.
4. Configure the service:
   - Name: skillicon-esp32-telegram (or another available name).
   - Branch: main.
   - Runtime / Language: Python 3.
   - Root Directory: server.
   - Build Command: pip install -r requirements.txt
   - Start Command: gunicorn --bind 0.0.0.0:$PORT app:app
   - Instance Type: Free.
   - Health Check Path: /health (under advanced settings, if offered).
5. No environment secrets are needed for this checkpoint. Do not paste bot tokens into source files or build/start commands.
6. Create/deploy the service. Wait until the deployment is Live. If it fails, inspect the deployment logs; do not proceed until the health check works.

## Verify
1. Open the public HTTPS URL shown by Render. It should show JSON with project Skillicon ESP32 Telegram and a message that the server is running but integration is not enabled yet.
2. Add /health to that same URL. Expected response: {"status":"ok"}.
3. Record the actual URL, deployment date, and successful response. Send only the public URL to continue the lesson; no credentials are needed.

## Explain to students
GitHub stores the code. Render installs its dependencies and runs it as an internet-accessible service. Flask defines the URLs and responses; Gunicorn runs the application. Render supplies the PORT environment variable and HTTPS. A health endpoint checks that the application can answer requests; it does not prove Telegram or hardware connectivity.

## Free hosting limits
The free web service can sleep after 15 minutes without incoming traffic; waking can take about a minute. Its runtime files are temporary and are lost on restarts/redeploys. Do not use local files as a permanent command queue or firmware store. Free usage quotas apply; inspect current limits in Render before the workshop.

## Troubleshooting
- app module not found or requirements file missing: check Root Directory is server.
- Port detection failure: check the start command exactly, including 0.0.0.0:$PORT.
- Repository missing: connect the correct GitHub account and grant repository access.
- Initial page loads slowly: allow the free service time to wake and retry.
- /health returns 404: check deployment branch and commit; ensure this lesson's server files are deployed.

## Next checkpoint
After the public URL is verified, add authenticated Telegram webhooks and device endpoints, then replace the direct Telegram polling sketch. Do not register a webhook while the old ESP32 sketch is still polling the same bot. Server-led command handling and OTA remain future work.

References checked 13 November 2026:
- https://render.com/docs/deploy-flask
- https://render.com/docs/free
