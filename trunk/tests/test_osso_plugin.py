import unittest
import osso
import os

class TestOssoPlugin(unittest.TestCase):
    def setUp(self):
        self.ctx = osso.Context("app", "0.1", False)
        os.system("cp libunittestplugin.so /usr/lib/hildon-control-panel")

    def tearDown(self):
        os.system("rm /usr/lib/hildon-control-panel/libunittestplugin.so")

    def test_constructor(self):
        app = osso.Plugin(self.ctx)
        self.assertTrue(isinstance(app, osso.Plugin))
        self.assertRaises(TypeError, osso.Plugin, None)

    def test_plugin_execute(self):
        plugin = osso.Plugin(self.ctx)
        plugin.plugin_execute("libunittestplugin.so", True)

    def test_plugin_save_state(self):
        plugin = osso.Plugin(self.ctx)
        plugin.plugin_save_state("libunittestplugin.so")
        pass


if __name__ == "__main__":
    unittest.main()
