import unittest
import osso

class TestOssoContext(unittest.TestCase):
    def test_constructor(self):
        ctx = osso.Context("test_osso", "0.1", False)
        self.assertTrue(isinstance(ctx, osso.Context))
        del ctx
        # third argument is ignored
        ctx = osso.Context("test_osso", "0.1", False)
        self.assertTrue(isinstance(ctx, osso.Context))
        # missing version
        self.assertRaises(TypeError, osso.Context, "test_osso")

if __name__ == "__main__":
    unittest.main()
