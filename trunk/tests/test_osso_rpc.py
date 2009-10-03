import os
import os.path
import time
import unittest

import osso

def receiver(program, version):
    return """#!/usr/bin/python2.5
import osso
import gtk

def callback_func(interface, method, arguments, user_data):
    print 'interface = "%%s", method = "%%s", arguments = %%s, user_data = %%s' %% (interface, method, arguments, user_data)
    gtk.main_quit()

if __name__ == "__main__":
    osso_c = osso.Context("%(program)s", "%(version)s", False)
    osso_rpc = osso.Rpc(osso_c)
    osso_rpc.set_rpc_callback("org.maemo.%(program)s",
                              "/org/maemo/%(program)s",
                              "org.maemo.%(program)s",
                              callback_func, "abc")
    gtk.main()
""" % locals()

def test_async_cb(interface, method, retval, user_data):
    # FIXME: this callback is never called even on older python-osso version
    # Needs to lack at a C example
    print "test_async_cb: interface = \"%s\", method = \"%s\", retval = %s, user_data = %s" % (interface, method, arguments, user_data)

class TestOssoRpc(unittest.TestCase):
    def setUp(self):
        self.program = "test_osso_receiver"
        self.async_program = "test_osso_async_receiver"
        self.ctx = osso.Context("test_osso", "0.1", False)

        for p in [self.program, self.async_program]:
            recv = file("/tmp/%s.py" % p, "w")
            recv.write(receiver(p, "0.1"))
            recv.close()
            os.chmod("/tmp/%s.py" % p, 0755)
            srv = file("/usr/share/dbus-1/services/%s.service" % p, "w")
            srv.write("[D-BUS Service]\nName=org.maemo.%s\nExec=%s\n" %
                      (p, "/tmp/%s.py" % p))
            srv.close()

    def tearDown(self):
        for p in [self.program, self.async_program]:
            os.remove("/tmp/%s.py" % p)
            os.remove("/usr/share/dbus-1/services/%s.service" % p)

    def test_constructor(self):
        rpc = osso.Rpc(self.ctx)
        self.assertTrue(isinstance(rpc, osso.Rpc))

    def test_rpc_run(self):
        rpc = osso.Rpc(self.ctx)
        rpc.rpc_run("org.maemo." + self.program,
                    "/org/maemo/" + self.program,
                    "org.maemo." + self.program,
                    "do_something",
                    ("xyz", -1, 1, True, False, 1.0),
                    True, False)

    def test_rpc_async_run(self):
        rpc = osso.Rpc(self.ctx)
        rpc.rpc_async_run("org.maemo." + self.async_program,
                          "/org/maemo/" + self.async_program,
                          "org.maemo." + self.async_program,
                          "do_something",
                          test_async_cb,
                          "abc",
                          ("xyz", -1, 1, True, False, 1.0))
        # wait some time to allow async RPC to finish
        time.sleep(2)

    def test_set_rpc_callback(self):
        rpc = osso.Rpc(self.ctx)
        # register callback
        rpc.set_rpc_callback("org.maemo." + self.program,
                             "/org/maemo/" + self.program,
                             "org.maemo." + self.program,
                             lambda i,m,a,u: None,
                             "abc")
        # unregister callback
        rpc.set_rpc_callback("org.maemo." + self.program,
                             "/org/maemo/" + self.program,
                             "org.maemo." + self.program,
                             None, "abc")
        # check for callable callback
        self.assertRaises(TypeError, rpc.set_rpc_callback,
                          "org.maemo." + self.program,
                          "/org/maemo/" + self.program,
                          "org.maemo." + self.program,
                          123, "abc")

    def test_get_set_timeout(self):
        rpc = osso.Rpc(self.ctx)
        rpc.set_rpc_timeout(100)
        i = rpc.get_rpc_timeout()
        self.assertEqual(i, 100)

if __name__ == "__main__":
    unittest.main()
