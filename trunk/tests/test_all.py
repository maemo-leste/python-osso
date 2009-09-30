import unittest
import sys
import glob
from os.path import dirname, basename, splitext

if __name__ == "__main__":
    tests_dir = dirname(sys.argv[0]) or "."
    sys.path.append(tests_dir)
    tests = glob.glob(tests_dir + "/test_*.py")
    tests = filter(lambda x: x != sys.argv[0], tests)
    tests = map(lambda x: splitext(basename(x))[0], tests)
    suites = []
    for t in tests:
        m = __import__(t)
        suites.append(unittest.defaultTestLoader.loadTestsFromModule(m))
    alltests = unittest.TestSuite(suites)
    unittest.TextTestRunner().run(alltests)
