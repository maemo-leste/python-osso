import unittest
import osso
import time

import gobject

class TestOssoTime(unittest.TestCase):
    def setUp(self):
        self.ctx = osso.Context("app", "0.1")

    def test_constructor_time(self):
        t = osso.Time(self.ctx)
        self.assertTrue(isinstance(t, osso.Time))
        self.assertRaises(TypeError, osso.Time, None)

    def test_set_time(self):
        t = osso.Time(self.ctx)
        t.set_time(int(time.time()))

    def notification_cb(self, user_data):
        user_data.called = True
        self.loop.quit()

    def app_quit(self):
        self.loop.quit()

    def test_set_notification(self):
        t = osso.Time(self.ctx)
        self.called = False
        t.set_time_notification_callback(self.notification_cb, self)
        t.set_time(int(time.time()))

        self.loop = gobject.MainLoop()
        cb_id = gobject.timeout_add(1000, self.app_quit)
        self.loop.run()
        gobject.source_remove(cb_id)
        self.assertEqual(self.called, True)

if __name__ == "__main__":
    unittest.main()
