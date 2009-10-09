import unittest
import osso

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
        def callback(*args):
            print "device_state_callback:", args
        state.set_device_state_callback(callback, memory_low=True)

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
