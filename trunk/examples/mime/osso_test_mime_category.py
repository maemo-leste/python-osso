#!/usr/bin/python2.5

import osso
import sys

# Provide a mime type (text/plain, image/png, ...) and the program will
# output the category associated.
def main():
    if len(sys.argv) < 2:
        print "Usage: %s <mimetype>" % sys.argv[0]

    osso_c = osso.Context("osso_test_mime_category", "0.0.1", False)
    mime = osso.Mime(osso_c)

    cat = mime.get_category_for_mime_type(sys.argv[1])

    print "category is: %s " % cat

if __name__ == "__main__":
    main()
