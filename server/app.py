"""Lesson 6: Render hosting checkpoint before connecting Telegram and ESP32."""
from flask import Flask, jsonify

app = Flask(__name__)


@app.get("/")
def index():
    return jsonify(
        project="Skillicon ESP32 Telegram",
        stage="Render hosting checkpoint",
        message="Server is running. Telegram and ESP32 integration is not enabled yet.",
        health_url="/health",
    )


@app.get("/health")
def health():
    return jsonify(status="ok"), 200
