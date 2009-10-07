import unittest
import osso
import os

plugin_source = """
#include <stdio.h>
#include <libosso.h>

osso_return_t execute(osso_context_t *osso, gpointer data,
                      gboolean user_activated)
{
    printf(\"unittestplugin: calling execute()\\n\");
    fflush(stdout);

    return OSSO_OK;
}

osso_return_t save_state(osso_context_t *osso, gpointer data)
{
    printf(\"unittestplugin: calling save_state()\\n\");
    fflush(stdout);

    return OSSO_OK;
}
"""

# only a single context must be created
context = osso.Context("test_osso", "0.1", False)

class TestOssoPlugin(unittest.TestCase):
    def setUp(self):
        self.ctx = context
        f = file("/tmp/libunittestplugin.c", "w")
        f.write(plugin_source)
        f.close()
        os.system("gcc -Wall -O2 -fPIC -shared /tmp/libunittestplugin.c " +
                  "-o /usr/lib/hildon-control-panel/libunittestplugin.so " +
                  "`pkg-config --cflags --libs libosso`")

    def tearDown(self):
        os.remove("/tmp/libunittestplugin.c")
        os.remove("/usr/lib/hildon-control-panel/libunittestplugin.so")

    def test_constructor(self):
        app = osso.Plugin(self.ctx)
        self.assertTrue(isinstance(app, osso.Plugin))
        self.assertRaises(TypeError, osso.Plugin, None)

    def test_plugin_execute(self):
        plugin = osso.Plugin(self.ctx)
        plugin.plugin_execute("libunittestplugin.so", True)

    def test_plugin_save_state(self):
        plugin = osso.Plugin(self.ctx)
        # XXX: unlike osso_cp_plugin_execute(), osso_cp_plugin_save_state()
        # requires the full path! Looks like a libosso bug.
        plugin.plugin_save_state("/usr/lib/hildon-control-panel/libunittestplugin.so")

if __name__ == "__main__":
    unittest.main()
