import unittest
import osso

class TestOssoMime(unittest.TestCase):
    def setUp(self):
        self.ctx = osso.Context("test_osso", "0.1")

    def test_constructor(self):
        mime = osso.Mime(self.ctx)
        self.assertTrue(isinstance(mime, osso.Mime))
        self.assertRaises(TypeError, osso.Mime, None)

    def mime_cb(self, user_data, args):
        pass

    def app_quit(self):
        self.loop.quit()

    def test_set_mime_callback(self):
        mime = osso.Mime(self.ctx)

        mime.set_mime_callback(self.mime_cb, self)
        mime.unset_mime_callback()

        mime.set_mime_callback(self.mime_cb, self)
        mime.unset_mime_callback_full(self.mime_cb, self)


if __name__ == "__main__":
    unittest.main()
