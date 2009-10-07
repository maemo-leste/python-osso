import unittest
import osso

class TestOssoSystemNotification(unittest.TestCase):
    def setUp(self):
        self.ctx = osso.Context("app", "0.1")

    def test_constructor_system_notification(self):
        systemnote = osso.SystemNote(self.ctx)
        self.assertTrue(isinstance(systemnote, osso.SystemNote))
        self.assertRaises(TypeError, osso.SystemNote, None)

if __name__ == "__main__":
    unittest.main()
