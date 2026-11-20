import os
import secrets
import time
from threading import Lock
from flask import Flask, jsonify, request, render_template

app = Flask(__name__)
app.config['MAX_CONTENT_LENGTH'] = 4096
lock = Lock()
state = {'report': None, 'seen': 0, 'pending': None, 'result': None}


def authorized(name):
    key = os.environ.get(name, '')
    return bool(key) and secrets.compare_digest(request.headers.get('Authorization', ''), 'Bearer ' + key)


def expire():
    if state['pending'] and time.time() > state['pending']['expires']:
        state['result'] = 'Command expired without confirmation'
        state['pending'] = None


@app.get('/')
def index():
    return render_template('index.html')


@app.get('/health')
def health():
    return jsonify(status='ok')


@app.get('/api/status')
def status():
    if not authorized('DASHBOARD_KEY'):
        return jsonify(error='Dashboard key missing or incorrect'), 401
    with lock:
        expire()
        return jsonify(online=time.time()-state['seen'] < 20,
                       report=state['report'], last_seen=state['seen'],
                       pending=state['pending'], result=state['result'])


@app.post('/api/command')
def command():
    if not authorized('DASHBOARD_KEY'):
        return jsonify(error='Unauthorized'), 401
    data = request.get_json(silent=True)
    if not isinstance(data, dict) or data.get('mode') not in ('ON','OFF','BLINK'):
        return jsonify(error='Use ON, OFF or BLINK'), 400
    with lock:
        expire()
        if time.time()-state['seen'] >= 20:
            return jsonify(error='Device offline; command not queued'), 409
        state['pending'] = {'id': secrets.token_hex(12), 'mode': data['mode'], 'expires': int(time.time())+30}
        state['result'] = 'Waiting for device confirmation'
        return jsonify(state['pending']), 202


@app.post('/api/device/sync')
def sync():
    if not authorized('DEVICE_KEY'):
        return jsonify(error='Unauthorized'), 401
    data = request.get_json(silent=True)
    if (not isinstance(data, dict) or data.get('mode') not in ('ON','OFF','BLINK')
        or type(data.get('led')) is not bool or data.get('source') not in ('BOOT','SERIAL','WEB')
        or type(data.get('uptime')) is not int or not 0 <= data['uptime'] <= 4294967295
        or type(data.get('rssi')) is not int or not -150 <= data['rssi'] <= 0
        or not isinstance(data.get('ack',''), str) or len(data.get('ack','')) > 64
        or data.get('outcome','') not in ('','applied','superseded')):
        return jsonify(error='Invalid telemetry'), 400
    with lock:
        expire()
        pending = state['pending']
        if pending and data.get('ack') == pending['id']:
            state['result'] = ('Device applied command' if data.get('outcome') == 'applied'
                               else 'Serial command superseded web command')
            state['pending'] = None
        state['report'] = {k: data[k] for k in ('mode','led','source','uptime','rssi')}
        state['seen'] = time.time()
        return jsonify(command=state['pending'])
