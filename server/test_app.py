import os
import time
import unittest
from unittest.mock import patch
from app import app, state

class DashboardTests(unittest.TestCase):
    def setUp(self):
        os.environ.update(DEVICE_KEY='device-test', DASHBOARD_KEY='web-test')
        state.update(report=None,seen=0,pending=None,result=None)
        self.c=app.test_client()
        self.web={'Authorization':'Bearer web-test'}
        self.device={'Authorization':'Bearer device-test'}
        self.report=dict(mode='OFF',led=False,source='BOOT',uptime=10,rssi=-50,ack='',outcome='')
    def sync(self,**kw):
        return self.c.post('/api/device/sync',json={**self.report,**kw},headers=self.device)
    def test_health_and_page(self):
        self.assertEqual(self.c.get('/health').json,{'status':'ok'})
        self.assertIn(b'ESP32 control lab',self.c.get('/').data)
    def test_auth_and_offline(self):
        self.assertEqual(self.c.get('/api/status').status_code,401)
        self.assertEqual(self.c.get('/api/status',headers=self.device).status_code,401)
        self.assertEqual(self.c.post('/api/command',json={'mode':'ON'},headers=self.web).status_code,409)
    def test_command_confirmation(self):
        self.sync()
        command=self.c.post('/api/command',json={'mode':'ON'},headers=self.web).json
        self.assertEqual(self.sync().json['command']['id'],command['id'])
        self.assertIsNone(self.sync(mode='ON',led=True,source='WEB',ack=command['id'],outcome='applied').json['command'])
        self.assertEqual(self.c.get('/api/status',headers=self.web).json['report']['mode'],'ON')
    def test_expiry_is_integer_for_esp32_parser(self):
        # ArduinoJson's `value | 0L` rejects a JSON floating-point number.
        with patch('app.time.time', return_value=1795000000.75):
            self.sync()
            command=self.c.post('/api/command',json={'mode':'ON'},headers=self.web).json
            self.assertIs(type(command['expires']), int)
            self.assertEqual(command['expires'],1795000030)
            delivered=self.sync().json['command']
            self.assertIs(type(delivered['expires']), int)
            self.assertGreater(delivered['expires'],1795000000)
    def test_expiration_and_invalid_report(self):
        self.sync()
        self.c.post('/api/command',json={'mode':'BLINK'},headers=self.web)
        state['pending']['expires']=time.time()-1
        self.assertIsNone(self.sync().json['command'])
        self.assertEqual(self.sync(led='false').status_code,400)
    def test_serial_and_superseded_ack(self):
        self.sync()
        old=self.c.post('/api/command',json={'mode':'ON'},headers=self.web).json
        new=self.c.post('/api/command',json={'mode':'OFF'},headers=self.web).json
        self.assertEqual(self.sync(ack=old['id'],outcome='applied').json['command']['id'],new['id'])
        self.assertIsNone(self.sync(mode='BLINK',source='SERIAL',ack=new['id'],outcome='superseded').json['command'])

if __name__=='__main__': unittest.main()
