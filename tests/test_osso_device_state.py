import unittest
import osso
import gobject
import dbus
import dbus.service
import dbus.mainloop.glib

class DbusTestObject(dbus.service.Object):
    """Create D-BUS service to emit a fake "save_unsaved_data_ind" signal."""

    @dbus.service.signal(dbus_interface="com.nokia.dsme.signal", signature="b")
    def save_unsaved_data_ind(self, value):
        pass

    @classmethod
    def emit_signal(klass):
        dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
        obj = klass(dbus.SystemBus(), "/com/nokia/dsme/signal")
        obj.save_unsaved_data_ind(True)

class TestOssoDeviceState(unittest.TestCase):
    def setUp(self):
        self.ctx = osso.Context("test_osso", "0.1", False)

    def test_constructor(self):
        state = osso.DeviceState(self.ctx)
        self.assertTrue(isinstance(state, osso.DeviceState))

    def test_display_state_on(self):
        state = osso.DeviceState(self.ctx)
        state.display_state_on()

    def test_display_blanking_pause(self):
        state = osso.DeviceState(self.ctx)
        state.display_blanking_pause()

    def test_set_device_state_callback(self):
        state = osso.DeviceState(self.ctx)
        main_loop = gobject.MainLoop()
        self.called = False

        def callback(shutdown, save_data, memory_low, system_inactivity, mode, user_data):
            print "set_device_state_callback:", locals()
            self.called = True
            main_loop.quit()
        state.set_device_state_callback(callback, save_data=True, user_data="abc")

        gobject.idle_add(DbusTestObject.emit_signal)
        source_id = gobject.timeout_add(1000, main_loop.quit)
        main_loop.run()
        gobject.source_remove(source_id)
        self.assertTrue(self.called)

    def test_set_display_event_cb(self):
        state = osso.DeviceState(self.ctx)
        def callback(*args):
            print "display_event_callback:", args
        state.set_display_event_cb(callback)
        self.assertTrue("OSSO_DISPLAY_ON" in dir(osso.device_state))
        self.assertTrue("OSSO_DISPLAY_OFF" in dir(osso.device_state))
        self.assertTrue("OSSO_DISPLAY_DIMMED" in dir(osso.device_state))

if __name__ == "__main__":
    unittest.main()
