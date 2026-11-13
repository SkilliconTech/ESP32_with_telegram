import unittest
from app import app


class HostingCheckpointTests(unittest.TestCase):
    def setUp(self):
        self.client = app.test_client()

    def test_health_is_successful_json(self):
        response = self.client.get("/health")
        self.assertEqual(response.status_code, 200)
        self.assertEqual(response.get_json(), {"status": "ok"})

    def test_home_explains_integration_is_not_enabled(self):
        response = self.client.get("/")
        self.assertEqual(response.status_code, 200)
        self.assertIn("not enabled yet", response.get_json()["message"])


if __name__ == "__main__":
    unittest.main()
