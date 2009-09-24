import unittest

import osso

class OssoContextText(unittest.TestCase):

    def testOssoContext(self):
        self.assertRaises(osso.OssoException, osso.Context,"org.com.testservice", "0.0.1", False)

if __name__ == '__main__':
    unittest.main()

