import os
import os.path
import time
import unittest

import osso

PROGRAM = "osso_test_receiver"
VERSION = "0.1"

receiver = """#!/usr/bin/python2.5
import osso
import gtk

def callback_func(interface, method, arguments, user_data):
    print 'interface = "%%s", method = "%%s", arguments = %%s, user_data = %%s' %% (interface, method, arguments, user_data)
    gtk.main_quit()

if __name__ == "__main__":
    osso_c = osso.Context("%(PROGRAM)s", "%(VERSION)s")
    osso_rpc = osso.Rpc(osso_c)
    osso_rpc.set_rpc_callback("org.maemo.%(PROGRAM)s",
                              "/org/maemo/%(PROGRAM)s",
                              "org.maemo.%(PROGRAM)s",
                              callback_func, "abc")
    gtk.main()
""" % locals()

def test_async_cb(interface, method, retval, user_data):
    print "test_async_cb: interface = \"%s\", method = \"%s\", retval = %s, user_data = %s" % (interface, method, arguments, user_data)

class TestOssoRpc(unittest.TestCase):
    def setUp(self):
        self.ctx = osso.Context(PROGRAM, VERSION)
        recv = file("/tmp/%s.py" % PROGRAM, "w")
        recv.write(receiver)
        recv.close()
        os.chmod("/tmp/%s.py" % PROGRAM, 0755)
        srv = file("/usr/share/dbus-1/services/%s.service" % PROGRAM, "w")
        srv.write("[D-BUS Service]\nName=org.maemo.%s\nExec=%s\n" %
                  (PROGRAM, "/tmp/%s.py" % PROGRAM))
        srv.close()

    def tearDown(self):
        os.remove("/tmp/%s.py" % PROGRAM)
        os.remove("/usr/share/dbus-1/services/%s.service" % PROGRAM)

    def test_constructor(self):
        rpc = osso.Rpc(self.ctx)
        self.assertTrue(isinstance(rpc, osso.Rpc))

    def test_rpc_run(self):
        rpc = osso.Rpc(self.ctx)
        rpc.rpc_run("org.maemo." + PROGRAM,
                    "/org/maemo/" + PROGRAM,
                    "org.maemo." + PROGRAM,
                    "do_something",
                    ("xyz", -1, 1, True, False, 1.0),
                    True, False)

# FIXME: conflicts with test_rpc_run() run
#    def test_rpc_async_run(self):
#        rpc = osso.Rpc(self.ctx)
#        rpc.rpc_async_run("org.maemo." + PROGRAM,
#                          "/org/maemo/" + PROGRAM,
#                          "org.maemo." + PROGRAM,
#                          "do_something",
#                          test_async_cb,
#                          "abc",
#                          ("xyz", -1, 1, True, False, 1.0))

    def test_set_rpc_callback(self):
        rpc = osso.Rpc(self.ctx)
        # register callback
        rpc.set_rpc_callback("org.maemo." + PROGRAM,
                             "/org/maemo/" + PROGRAM,
                             "org.maemo." + PROGRAM,
                             lambda i,m,a,u: None,
                             "abc")
        # unregister callback
        rpc.set_rpc_callback("org.maemo." + PROGRAM,
                             "/org/maemo/" + PROGRAM,
                             "org.maemo." + PROGRAM,
                             None, "abc")
        # check for callable callback
        self.assertRaises(TypeError, rpc.set_rpc_callback,
                          "org.maemo." + PROGRAM,
                          "/org/maemo/" + PROGRAM,
                          "org.maemo." + PROGRAM,
                          123, "abc")

if __name__ == "__main__":
    unittest.main()
