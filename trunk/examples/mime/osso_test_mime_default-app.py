#!/usr/bin/python2.5

import sys
from gnome import gnomevfs


# Provide a mime type (text/plain, image/png, ...) and the program will
# output informations about the current default program for the given
# mime types.
def main():
    if len(sys.argv) < 2:
        print "Usage: %s <mimetype>" % sys.argv[0]

    app = gnomevfs.mime_get_default_application(sys.argv[1])

    if app == None:
        print "No mime details associated with %s" % sys.argv[1]
        return

    print "Mime details for %s" % sys.argv[1]
    print app

if __name__ == "__main__":
    main()
