import unittest
import osso

class TestOssoApplication(unittest.TestCase):
    def setUp(self):
        self.ctx = osso.Context("app", "0.1", False)

    def test_constructor(self):
        app = osso.Application(self.ctx)
        self.assertTrue(isinstance(app, osso.Application))
        self.assertRaises(TypeError, osso.Application, None)

    def test_application_top(self):
        app = osso.Application(self.ctx)
        app.application_top("browse")

if __name__ == "__main__":
    unittest.main()
