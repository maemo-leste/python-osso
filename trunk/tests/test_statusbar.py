import unittest
import osso

class TestOssoStateSavingAutosave(unittest.TestCase):
    def setUp(self):
        self.ctx = osso.Context("app", "0.1")

    def test_constructor_status_bar(self):
        statusbar = osso.StatusBar(self.ctx)
        self.assertTrue(isinstance(statusbar, osso.StatusBar))
        self.assertRaises(TypeError, osso.StatusBar, None)

    def test_send_event(self):
        statusbar = osso.StatusBar(self.ctx)
        #statusbar.send_event("display", 1, 1, "")

if __name__ == "__main__":
    unittest.main()
