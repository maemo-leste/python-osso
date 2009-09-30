import unittest
import osso

class TestOssoContext(unittest.TestCase):
    def test_constructor(self):
        ctx = osso.Context("app", "0.1")
        self.assertTrue(isinstance(ctx, osso.Context))
        del ctx
        # third argument is ignored
        ctx = osso.Context("app", "0.1", None)
        self.assertTrue(isinstance(ctx, osso.Context))
        # missing version
        self.assertRaises(TypeError, osso.Context, "app")

if __name__ == "__main__":
    unittest.main()
