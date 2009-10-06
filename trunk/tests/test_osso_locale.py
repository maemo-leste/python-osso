import unittest
import osso

import gobject

class TestOssoLocale(unittest.TestCase):
    def setUp(self):
        self.ctx = osso.Context("app", "0.1")

    def test_constructor_status_bar(self):
        locale = osso.Locale(self.ctx)
        self.assertTrue(isinstance(locale, osso.Locale))
        self.assertRaises(TypeError, osso.Locale, None)

    def locale_changed(self, new_locale, user_data):
        user_data.new_locale = new_locale

    def app_quit(self):
        self.loop.quit()

    def test_set_localet(self):
        locale = osso.Locale(self.ctx)
        locale.set_locale_notification_callback(self.locale_changed, self)

        self.new_locale = ""
        locale.set_locale("en_GB")

        self.loop = gobject.MainLoop()
        gobject.idle_add(self.app_quit)
        self.loop.run()

        self.assertEqual(self.new_locale, "en_GB")


if __name__ == "__main__":
    unittest.main()
