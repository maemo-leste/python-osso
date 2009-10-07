import os
import unittest

import osso

def receiver(program, version, service, object_path, interface):
    return """#!/usr/bin/python2.5
import osso
import gobject

main_loop = gobject.MainLoop()

def callback_func(*args):
    print "%(program)s:", args
    main_loop.quit()

osso_c = osso.Context("%(program)s", "%(version)s", False)
osso_rpc = osso.Rpc(osso_c)
osso_rpc.set_rpc_callback("%(service)s",
                          "%(object_path)s",
                          "%(interface)s",
                          callback_func, "abc")

cb_id = gobject.timeout_add(1000, main_loop.quit)
main_loop.run()
gobject.source_remove(cb_id)

""" % locals()

def test_async_cb(*args):
    print "test_async_cb:", args

class TestOssoRpc(unittest.TestCase):
    def setUp(self):
        self.program = "test_osso_receiver"
        self.async_program = "test_osso_async_receiver"

        for p in [self.program, self.async_program]:
            self._set_dbus_data(p)
            recv = file("/tmp/%s.py" % p, "w")
            recv.write(receiver(p, "0.1", self.service, self.object_path,
                                self.interface))
            recv.close()
            os.chmod("/tmp/%s.py" % p, 0755)
            srv = file("/usr/share/dbus-1/services/%s.service" % p, "w")
            srv.write("[D-BUS Service]\nName=%s\nExec=%s\n" %
                      (self.service, "/tmp/%s.py" % p))
            srv.close()

        self.ctx = osso.Context("test_osso", "0.1", False)

    def tearDown(self):
        for p in [self.program, self.async_program]:
            os.remove("/tmp/%s.py" % p)
            os.remove("/usr/share/dbus-1/services/%s.service" % p)

    def _set_dbus_data(self, program):
        self.service = "com.nokia." + program
        self.object_path = "/com/nokia/" + program
        self.interface = self.service

    def test_constructor(self):
        rpc = osso.Rpc(self.ctx)
        self.assertTrue(isinstance(rpc, osso.Rpc))

    def test_rpc_run(self):
        self._set_dbus_data(self.program)
        rpc = osso.Rpc(self.ctx)
        ret = rpc.rpc_run(self.service,
                          self.object_path,
                          self.interface,
                          "do_something",
                          ("xyz", -1, 1, True, False, 1.0),
                          True, False)

    def test_rpc_run_with_defaults(self):
        # Delay for some time before calling rpc_run_with_defaults().
        # Otherwise, this error might occur:
        # Message did not receive a reply (timeout by message bus)
        import time
        time.sleep(0.5)
        rpc = osso.Rpc(self.ctx)
        ret = rpc.rpc_run_with_defaults(self.program,
                                        "do_something",
                                        ("xyz", -1, 1, True, False, 1.0),
                                        True)

    def test_rpc_async_run(self):
        self._set_dbus_data(self.async_program)
        rpc = osso.Rpc(self.ctx)
        rpc.rpc_async_run(self.service,
                          self.object_path,
                          self.interface,
                          "do_something",
                          test_async_cb,
                          "abc",
                          ("xyz", -1, 1, True, False, 1.0))

    def test_set_rpc_callback(self):
        self._set_dbus_data(self.program)
        rpc = osso.Rpc(self.ctx)
        # register callback
        rpc.set_rpc_callback(self.service,
                             self.object_path,
                             self.interface,
                             lambda i,m,a,u: None,
                             "abc")
        # unregister callback
        rpc.set_rpc_callback(self.service,
                             self.object_path,
                             self.interface,
                             None, "abc")
        # check for callable callback
        self.assertRaises(TypeError, rpc.set_rpc_callback,
                          self.service,
                          self.object_path,
                          self.interface,
                          123, "abc")

    def test_set_rpc_default_callback(self):
        rpc = osso.Rpc(self.ctx)
        # register callback
        rpc.set_rpc_default_callback(lambda i,m,a,u: None, "abc")
        # unregister callback
        rpc.set_rpc_default_callback(None, "abc")
        # check for callable callback
        self.assertRaises(TypeError, rpc.set_rpc_default_callback, 123, "abc")

    def test_get_set_timeout(self):
        rpc = osso.Rpc(self.ctx)
        rpc.set_rpc_timeout(100)
        i = rpc.get_rpc_timeout()
        self.assertEqual(i, 100)

if __name__ == "__main__":
    unittest.main()
