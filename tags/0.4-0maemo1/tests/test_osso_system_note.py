import os
import os.path
import unittest

import osso

receiver_source = """#!/usr/bin/python2.5
import osso
import gobject

main_loop = gobject.MainLoop()

def callback(*args):
    print "callback:", args
    main_loop.quit()

print "*** Fake notification system"

context = osso.Context("test_notification", "0.1")
rpc = osso.Rpc(context)
rpc.set_rpc_callback("org.freedesktop.Notifications",
                     "/org/freedesktop/Notifications",
                     "org.freedesktop.Notifications",
                     callback)

cb_id = gobject.timeout_add(1000, main_loop.quit)
main_loop.run()
gobject.source_remove(cb_id)
"""

class TestOssoSystemNotification(unittest.TestCase):
    def setUp(self):
        self.ctx = osso.Context("test_osso", "0.1")
        self.orig_service = "/usr/share/dbus-1/services/org.freedesktop.Notifications.service"
        self.test_service = "/usr/share/dbus-1/services/test_osso.service"
        self.receiver = "/tmp/test_osso.py"

        if os.path.exists(self.orig_service):
            os.rename(self.orig_service, self.orig_service + ".orig")
        f = file(self.receiver, "w")
        f.write(receiver_source)
        f.close()
        os.chmod(self.receiver, 0755)
        f = file(self.test_service, "w")
        f.write("[D-BUS Service]\n" +
                "Name=org.freedesktop.Notifications\n" +
                "Exec=" + self.receiver)
        f.close()

    def tearDown(self):
        os.remove(self.receiver)
        os.remove(self.test_service)
        if os.path.exists(self.orig_service + ".orig") and \
            not os.path.exists(self.orig_service):
            os.rename(self.orig_service + ".orig", self.orig_service)

    def test_constructor_system_notification(self):
        systemnote = osso.SystemNote(self.ctx)
        self.assertTrue(isinstance(systemnote, osso.SystemNote))
        self.assertRaises(TypeError, osso.SystemNote, None)

    def test_system_note_dialog(self):
        note = osso.SystemNote(self.ctx)
        note.system_note_dialog("test message", "warning")

    def test_system_note_infoprint(self):
        # Delay for some time before calling rpc_run_with_defaults().
        # Otherwise, this error might occur:
        # Message did not receive a reply (timeout by message bus)
        import time
        time.sleep(0.5)
        note = osso.SystemNote(self.ctx)
        note.system_note_infoprint("test text")

if __name__ == "__main__":
    unittest.main()
