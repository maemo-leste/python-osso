#!/usr/bin/python2.5

import osso
import sys

# Provide a mime type (text/plain, image/png, ...) and the program will
# output the category associated.
def main():
    if len(sys.argv) < 2:
        print "Usage: %s <uri> [<mimetype>]" % sys.argv[0]

    osso_c = osso.Context("osso_test_mime_category", "0.0.1", False)
    mime = osso.Mime(osso_c)

    print "Testing osso.Mime.open_file with uri:'%s'" % sys.argv[1]
    ret = mime.open_file(sys.argv[1])

    print "Testing osso.Mime.open_file_list with uri:'%s'" % sys.argv[1]
    ret = mime.open_file_list([sys.argv[1]])


if __name__ == "__main__":
    main()
