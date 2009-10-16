#!/usr/bin/python2.5

import osso

def main():
    osso_c = osso.Context("test_plugin_app", "0.0.1", False)

    plugin = osso.Plugin(osso_c)

    plugin.plugin_execute("libtestplugin.so", True)

if __name__ == "__main__":
    main()
